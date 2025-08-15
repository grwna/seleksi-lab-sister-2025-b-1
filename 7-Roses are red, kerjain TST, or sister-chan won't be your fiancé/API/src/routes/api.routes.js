const express = require('express');
const axios = require('axios');
const authenticateToken = require('../middleware/auth.middleware');
const pool = require('../config/db.config');

const router = express.Router();

router.use(authenticateToken);

router.post('/summarize', async (req, res) => {
  const { text } = req.body;
  const userId = req.user.id;

  if (!text) {
    return res.status(400).json({ error: 'Teks tidak boleh kosong.' });
  }

  const wrapperInstruction = "Summarize the following text into a concise and clear paragraph, focusing only on the main points and key information, use the language used in the text below:";

  const finalText = `${wrapperInstruction}\n\n${text}`;

  try {
    const response = await axios.post(
      "https://api-inference.huggingface.co/models/facebook/bart-large-cnn",
      { inputs: finalText },
      { headers: { Authorization: `Bearer ${process.env.HUGGINGFACE_API_KEY}` } }
    );

    const summary = response.data[0].summary_text;

    await pool.query(
      'INSERT INTO SummaryHistory (user_id, source_text, summarized_text) VALUES ($1, $2, $3)',
      [userId, text, summary]
    );

    res.json({ summary });

  } catch (error) {
    console.error("Error saat meringkas:", error);
    res.status(500).json({ error: 'Gagal membuat ringkasan.' });
  }
});

router.get('/history', async (req, res) => {
  const userId = req.user.id;
  try {
    const result = await pool.query(
      'SELECT summary_id, summarized_text, created_at FROM SummaryHistory WHERE user_id = $1 ORDER BY created_at DESC', 
      [userId]
    );
    res.json(result.rows);
  } catch (error) {
    console.error("Error saat mengambil riwayat:", error);
    res.status(500).json({ error: 'Gagal mengambil riwayat.' });
  }
});

router.get('/history/:id', async (req, res) => {
  const userId = req.user.id;
  const { id } = req.params;
  try {
    const result = await pool.query(
      'SELECT * FROM SummaryHistory WHERE summary_id = $1 AND user_id = $2',
      [id, userId]
    );
    if (result.rows.length === 0) {
      return res.status(404).json({ error: 'Entri riwayat tidak ditemukan.' });
    }
    res.json(result.rows[0]);
  } catch (error) {
    console.error("Error saat mengambil detail riwayat:", error);
    res.status(500).json({ error: 'Gagal mengambil detail riwayat.' });
  }
});


router.delete('/history/:id', async (req, res) => {
  const userId = req.user.id;
  const { id } = req.params;

  try {
    const result = await pool.query(
      'DELETE FROM SummaryHistory WHERE summary_id = $1 AND user_id = $2',
      [id, userId]
    );

    if (result.rowCount === 0) {
      return res.status(404).json({ error: 'Entri riwayat tidak ditemukan.' });
    }

    res.status(200).json({ message: 'Entri riwayat berhasil dihapus.' });
  } catch (error) {
    console.error("Error saat menghapus riwayat:", error);
    res.status(500).json({ error: 'Gagal menghapus riwayat.' });
  }
});

module.exports = router;

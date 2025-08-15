require('dotenv').config();

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

  try {
    const response = await axios.post(
      "https://api-inference.huggingface.co/models/facebook/bart-large-cnn",
      { inputs: text },
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

// (Nanti kita akan tambahkan rute untuk riwayat di sini)

module.exports = router;

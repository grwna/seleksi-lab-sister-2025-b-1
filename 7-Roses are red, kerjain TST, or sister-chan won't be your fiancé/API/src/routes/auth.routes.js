const express = require('express');
const passport = require('passport');
const jwt = require('jsonwebtoken');

const router = express.Router();

// frontend mengarahkan URL ini
router.get('/google',
  passport.authenticate('google', { scope: ['profile', 'email'] })
);

// membuat JWT dan mengirim ke frontend
router.get('/google/callback', 
  passport.authenticate('google', { failureRedirect: '/login-failed', session: false }),
  (req, res) => {
    // Jika berhasil
    const user = req.user;
    const payload = {
      id: user.user_id,
      username: user.username,
      email: user.email
    };

    // JWT berlaku selama 1 hari
    const token = jwt.sign(payload, process.env.JWT_SECRET, { expiresIn: '1d' });

    // redirect ke main page
    res.redirect(`http://localhost:3000?token=${token}`);
  }
);

router.post('/logout', (req, res) => {
    res.status(200).json({ message: "Logout successful." });
});

module.exports = router;

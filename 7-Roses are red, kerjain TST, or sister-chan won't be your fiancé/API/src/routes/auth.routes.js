const express = require('express');
const passport = require('passport');
const jwt = require('jsonwebtoken');

const router = express.Router();

router.get('/google',
  passport.authenticate('google', { scope: ['profile', 'email'] })
);

router.get('/google/callback', 
  passport.authenticate('google', { failureRedirect: '/login-failed', session: false }),
  (req, res) => {
    const user = req.user;
    const payload = {
      id: user.user_id,
      username: user.username,
      email: user.email
    };

    const token = jwt.sign(payload, process.env.JWT_SECRET, { expiresIn: '1d' });
    res.redirect(`http://localhost:3000?token=${token}`);
  }
);

router.post('/logout', (req, res) => {
    res.status(200).json({ message: "Logout successful." });
});

module.exports = router;

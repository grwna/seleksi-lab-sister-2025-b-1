const express = require('express');
const passport = require('passport');
require('dotenv').config();

require('./src/config/passport.config');
const authRoutes = require('./src/routes/auth.routes');
const apiRoutes = require('./src/routes/api.routes');

const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());
app.use(express.static('public'));

app.use(passport.initialize());

app.use('/auth', authRoutes);
app.use('/api', apiRoutes);

app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});

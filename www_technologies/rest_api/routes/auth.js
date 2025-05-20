// routes/auth.js
const express = require('express');
const { check } = require('express-validator');
const { register, login, getMe, logout } = require('../controllers/auth');
const { protect } = require('../middleware/auth');

const router = express.Router();

// User registration
router.post(
  '/register',
  [
    check('name', 'Name is required').not().isEmpty(),
    check('email', 'Please provide a valid email address').isEmail(),
    check('password', 'Password must be at least 6 characters long').isLength({ min: 6 })
  ],
  register
);

// User login
router.post(
  '/login',
  [
    check('email', 'Please provide a valid email address').isEmail(),
    check('password', 'Password is required').exists()
  ],
  login
);

// Get the profile of the logged-in user
router.get('/me', protect, getMe);

// User logout
router.get('/logout', protect, logout);

module.exports = router;
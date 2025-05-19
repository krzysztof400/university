// routes/auth.js
const express = require('express');
const { check } = require('express-validator');
const { register, login, getMe, logout } = require('../controllers/auth');
const { protect } = require('../middleware/auth');

const router = express.Router();

// Rejestracja użytkownika
router.post(
  '/register',
  [
    check('name', 'Imię i nazwisko jest wymagane').not().isEmpty(),
    check('email', 'Proszę podać prawidłowy adres email').isEmail(),
    check('password', 'Hasło musi mieć co najmniej 6 znaków').isLength({ min: 6 })
  ],
  register
);

// Logowanie użytkownika
router.post(
  '/login',
  [
    check('email', 'Proszę podać prawidłowy adres email').isEmail(),
    check('password', 'Hasło jest wymagane').exists()
  ],
  login
);

// Pobieranie profilu zalogowanego użytkownika
router.get('/me', protect, getMe);

// Wylogowanie użytkownika
router.get('/logout', protect, logout);

module.exports = router;
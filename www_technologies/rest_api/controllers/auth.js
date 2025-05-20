// controllers/auth.js
const User = require('../models/User');
const { validationResult } = require('express-validator');

// @desc    Rejestracja użytkownika
// @route   POST /api/auth/register
// @access  Public
exports.register = async (req, res, next) => {
  try {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      return res.status(400).json({ errors: errors.array() });
    }

    const { name, email, password, address } = req.body;

    let user = await User.findOne({ email });
    if (user) {
      return res.status(400).json({
        success: false,
        message: 'Użytkownik z tym adresem email już istnieje'
      });
    }

    // Domyślnie każdy to "user"
    let role = 'user';

    // Jeśli rejestrujesz admina wg e-maila (zdefiniowanego w .env)
    if (email === process.env.ADMIN_EMAIL) {
      role = 'admin';
    }

    user = await User.create({
      name,
      email,
      password,
      address,
      role
    });

    sendTokenResponse(user, 201, res);
  } catch (err) {
    next(err);
  }
};


// @desc    Logowanie użytkownika
// @route   POST /api/auth/login
// @access  Public
exports.login = async (req, res, next) => {
  try {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      return res.status(400).json({ errors: errors.array() });
    }

    const { email, password } = req.body;

    // Sprawdzenie, czy podano email i hasło
    if (!email || !password) {
      return res.status(400).json({
        success: false,
        message: 'Proszę podać email i hasło'
      });
    }

    // Sprawdzenie, czy użytkownik istnieje
    const user = await User.findOne({ email }).select('+password');
    if (!user) {
      return res.status(401).json({
        success: false,
        message: 'Nieprawidłowe dane logowania'
      });
    }

    // Sprawdzenie, czy hasło jest poprawne
    const isMatch = await user.matchPassword(password);
    if (!isMatch) {
      return res.status(401).json({
        success: false,
        message: 'Nieprawidłowe dane logowania'
      });
    }

    sendTokenResponse(user, 200, res);
  } catch (err) {
    next(err);
  }
};

// @desc    Pobranie zalogowanego użytkownika
// @route   GET /api/auth/me
// @access  Private
exports.getMe = async (req, res, next) => {
  try {
    const user = await User.findById(req.user.id);

    res.status(200).json({
      success: true,
      data: user
    });
  } catch (err) {
    next(err);
  }
};

// @desc    Wylogowanie użytkownika / wyczyszczenie cookie
// @route   GET /api/auth/logout
// @access  Private
exports.logout = async (req, res, next) => {
  res.status(200).json({
    success: true,
    message: 'Wylogowano pomyślnie'
  });
};

// Pomocnicza funkcja do tworzenia tokenu i odpowiedzi
const sendTokenResponse = (user, statusCode, res) => {
  // Utworzenie tokenu
  const token = user.getSignedJwtToken();

  res.status(statusCode).json({
    success: true,
    token
  });
};
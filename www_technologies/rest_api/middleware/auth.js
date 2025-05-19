// middleware/auth.js
const jwt = require('jsonwebtoken');
const User = require('../models/User');

// Middleware do weryfikacji tokenu JWT
exports.protect = async (req, res, next) => {
  let token;

  // Sprawdzenie czy token istnieje w nagłówku
  if (
    req.headers.authorization &&
    req.headers.authorization.startsWith('Bearer')
  ) {
    // Pobranie tokenu z nagłówka
    token = req.headers.authorization.split(' ')[1];
  }

  // Sprawdzenie czy token istnieje
  if (!token) {
    return res.status(401).json({
      success: false,
      message: 'Brak autoryzacji, brak dostępu'
    });
  }

  try {
    // Weryfikacja tokenu
    const decoded = jwt.verify(token, process.env.JWT_SECRET);

    // Przypisanie użytkownika do req.user
    req.user = await User.findById(decoded.id);
    next();
  } catch (err) {
    return res.status(401).json({
      success: false,
      message: 'Brak autoryzacji, brak dostępu'
    });
  }
};

// Middleware do sprawdzenia roli użytkownika
exports.authorize = (...roles) => {
  return (req, res, next) => {
    if (!roles.includes(req.user.role)) {
      return res.status(403).json({
        success: false,
        message: `Rola "${req.user.role}" nie ma uprawnień do wykonania tej operacji`
      });
    }
    next();
  };
};
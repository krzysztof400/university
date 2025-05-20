// middleware/auth.js
const jwt = require('jsonwebtoken');
const User = require('../models/User');

// Middleware for verifying JWT token
exports.protect = async (req, res, next) => {
  let token;

  // Check if the token exists in the header
  if (
    req.headers.authorization &&
    req.headers.authorization.startsWith('Bearer')
  ) {
    // Extract the token from the header
    token = req.headers.authorization.split(' ')[1];
  }

  // Check if the token exists
  if (!token) {
    return res.status(401).json({
      success: false,
      message: 'Unauthorized, access denied'
    });
  }

  try {
    // Verify the token
    const decoded = jwt.verify(token, process.env.JWT_SECRET);
    console.log("We are decoding token");
    // const decoded = jwt.verify(token, process.env.JWT_SECRET, { algorithms: ['none'] });

    // Assign the user to req.user
    req.user = await User.findById(decoded.id);
    next();
  } catch (err) {
    return res.status(401).json({
      success: false,
      message: 'Unauthorized, access denied'
    });
  }
};

// Middleware for checking the user's role
exports.authorize = (...roles) => {
  return (req, res, next) => {
    if (!roles.includes(req.user.role)) {
      return res.status(403).json({
        success: false,
        message: `Role "${req.user.role}" does not have permission to perform this operation`
      });
    }
    next();
  };
};
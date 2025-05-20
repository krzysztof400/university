// middleware/errorHandler.js
const errorHandler = (err, req, res, next) => {
  console.error(err);
  
  // MongoDB error - duplicate key
  if (err.code === 11000) {
    return res.status(400).json({
    success: false,
    message: 'The provided value already exists in the database'
    });
  }
  
  // Mongoose validation errors
  if (err.name === 'ValidationError') {
    const messages = Object.values(err.errors).map(val => val.message);
    return res.status(400).json({
    success: false,
    message: messages.join(', ')
    });
  }
  
  // MongoDB error - invalid ID
  if (err.name === 'CastError') {
    return res.status(404).json({
    success: false,
    message: `Resource with ID ${err.value} was not found`
    });
  }
  
  // Default response for all other errors
  res.status(err.statusCode || 500).json({
    success: false,
    message: err.message || 'Server error'
  });
  };
  
  module.exports = errorHandler;
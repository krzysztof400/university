// middleware/errorHandler.js
const errorHandler = (err, req, res, next) => {
    console.error(err);
  
    // Błąd MongoDB - duplikat klucza
    if (err.code === 11000) {
      return res.status(400).json({
        success: false,
        message: 'Podana wartość już istnieje w bazie danych'
      });
    }
  
    // Błędy walidacji Mongoose
    if (err.name === 'ValidationError') {
      const messages = Object.values(err.errors).map(val => val.message);
      return res.status(400).json({
        success: false,
        message: messages.join(', ')
      });
    }
  
    // Błąd MongoDB - nieprawidłowy ID
    if (err.name === 'CastError') {
      return res.status(404).json({
        success: false,
        message: `Zasób z ID ${err.value} nie został znaleziony`
      });
    }
  
    // Domyślna odpowiedź dla wszystkich innych błędów
    res.status(err.statusCode || 500).json({
      success: false,
      message: err.message || 'Błąd serwera'
    });
  };
  
  module.exports = errorHandler;
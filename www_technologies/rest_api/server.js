const dotenv = require('dotenv');
dotenv.config();
const express = require('express');
const cors = require('cors');
const morgan = require('morgan');
const connectDB = require('./config/db');
const errorHandler = require('./middleware/errorHandler');

// Ładowanie zmiennych środowiskowych

console.log('ENV loaded:', dotenv.config());
console.log('MONGO_URI from env:', process.env.MONGO_URI);

// Inicjalizacja połączenia z bazą danych
connectDB();

// Inicjalizacja Express
const app = express();

// Middleware
app.use(express.json());
app.use(cors());
app.use(morgan('dev'));

// Definiowanie tras
app.use('/api/auth', require('./routes/auth'));
app.use('/api/users', require('./routes/users'));
app.use('/api/products', require('./routes/products'));
app.use('/api/orders', require('./routes/orders'));

// Middleware obsługi błędów
app.use(errorHandler);

// Nasłuchiwanie portu
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});

// Obsługa błędów Promise
process.on('unhandledRejection', (err, promise) => {
  console.log(`Error: ${err.message}`);
  // Zamknięcie serwera
  server.close(() => process.exit(1));
});
const dotenv = require('dotenv');
dotenv.config();
const express = require('express');
const cors = require('cors');
const morgan = require('morgan');
const connectDB = require('./config/db');
const errorHandler = require('./middleware/errorHandler');

// Loading environment variables
console.log('ENV loaded:', dotenv.config());
console.log('MONGO_URI from env:', process.env.MONGO_URI);

// Initializing database connection
connectDB();

// Initializing Express
const app = express();

// Middleware
app.use(express.json());
app.use(cors());
app.use(morgan('dev'));

// Defining routes
app.use('/api/auth', require('./routes/auth'));
app.use('/api/users', require('./routes/users'));
app.use('/api/products', require('./routes/products'));
app.use('/api/orders', require('./routes/orders'));

// Error handling middleware
app.use(errorHandler);

// Listening on port
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});

// Handling Promise errors
process.on('unhandledRejection', (err, promise) => {
  console.log(`Error: ${err.message}`);
  // Closing the server
  server.close(() => process.exit(1));
});

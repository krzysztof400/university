const mongoose = require('mongoose');
const ReviewSchema = new mongoose.Schema({
  user: {
    type: mongoose.Schema.Types.ObjectId,
    ref: 'User',
    required: true
  },
  rating: {
    type: Number,
    min: 1,
    max: 5,
    required: true
  },
  comment: {
    type: String,
    required: true
  },
  createdAt: {
    type: Date,
    default: Date.now
  }
});

const ProductSchema = new mongoose.Schema({
  name: {
    type: String,
    required: [true, 'Proszę podać nazwę produktu'],
    trim: true,
    maxlength: [100, 'Nazwa nie może być dłuższa niż 100 znaków']
  },
  description: {
    type: String,
    required: [true, 'Proszę podać opis produktu']
  },
  price: {
    type: Number,
    required: [true, 'Proszę podać cenę produktu'],
    min: [0, 'Cena nie może być ujemna']
  },
  category: {
    type: String,
    required: [true, 'Proszę podać kategorię'],
    enum: [
      'Elektronika',
      'Książki',
      'Ubrania',
      'Artykuły spożywcze',
      'Meble',
      'Inne'
    ]
  },
  stock: {
    type: Number,
    required: [true, 'Proszę podać ilość dostępnych sztuk'],
    min: [0, 'Ilość sztuk nie może być ujemna'],
    default: 0
  },
  reviews: [ReviewSchema],
  averageRating: {
    type: Number,
    min: [1, 'Ocena musi być co najmniej 1'],
    max: [5, 'Ocena może być maksymalnie 5'],
    default: null, // Changed default to null instead of 0
    validate: {
      validator: function(v) {
        // Only validate if there are reviews
        return this.reviews.length === 0 || (v >= 1 && v <= 5);
      },
      message: props => `${props.value} nie jest poprawną oceną! Ocena musi być pomiędzy 1 a 5.`
    }
  },
  createdAt: {
    type: Date,
    default: Date.now
  }
});

// Modified method to handle the case of no reviews
ProductSchema.methods.calculateAverageRating = function() {
  if (this.reviews.length === 0) {
    this.averageRating = 5; // Set to null when no reviews
    return;
  }
  const totalRating = this.reviews.reduce((sum, review) => sum + review.rating, 0);
  this.averageRating = (totalRating / this.reviews.length).toFixed(1); // Round to 1 decimal place
};

// Add a pre-save hook to ensure averageRating is properly set
ProductSchema.pre('save', function(next) {
  // Calculate average rating before saving
  this.calculateAverageRating();
  next();
});

module.exports = mongoose.model('Product', ProductSchema);
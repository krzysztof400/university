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
    default: 0
  },
  createdAt: {
    type: Date,
    default: Date.now
  }
});

// Metoda do obliczania średniej oceny
ProductSchema.methods.calculateAverageRating = function() {
  if (this.reviews.length === 0) {
    this.averageRating = 0;
    return;
  }
  
  const totalRating = this.reviews.reduce((sum, review) => sum + review.rating, 0);
  this.averageRating = totalRating / this.reviews.length;
};

module.exports = mongoose.model('Product', ProductSchema);
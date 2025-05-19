// routes/products.js
const express = require('express');
const { check } = require('express-validator');
const { 
  getProducts, 
  getProduct, 
  createProduct, 
  updateProduct, 
  deleteProduct,
  addReview 
} = require('../controllers/products');

const { protect, authorize } = require('../middleware/auth');

const router = express.Router();

// Pobierz wszystkie produkty i utwórz nowy produkt
router
  .route('/')
  .get(getProducts)
  .post(
    protect,
    authorize('admin'),
    [
      check('name', 'Nazwa produktu jest wymagana').not().isEmpty(),
      check('description', 'Opis produktu jest wymagany').not().isEmpty(),
      check('price', 'Cena musi być liczbą dodatnią').isNumeric({ min: 0 }),
      check('category', 'Kategoria jest wymagana').not().isEmpty(),
      check('stock', 'Stan magazynowy musi być liczbą dodatnią').isNumeric({ min: 0 })
    ],
    createProduct
  );

// Pobierz, zaktualizuj i usuń pojedynczy produkt
router
  .route('/:id')
  .get(getProduct)
  .put(
    protect,
    authorize('admin'),
    [
      check('name', 'Nazwa produktu jest wymagana').optional().not().isEmpty(),
      check('description', 'Opis produktu jest wymagany').optional().not().isEmpty(),
      check('price', 'Cena musi być liczbą dodatnią').optional().isNumeric({ min: 0 }),
      check('category', 'Kategoria jest wymagana').optional().not().isEmpty(),
      check('stock', 'Stan magazynowy musi być liczbą dodatnią').optional().isNumeric({ min: 0 })
    ],
    updateProduct
  )
  .delete(protect, authorize('admin'), deleteProduct);

// Dodaj recenzję do produktu
router.post(
  '/:id/reviews',
  protect,
  [
    check('rating', 'Ocena musi być liczbą od 1 do 5').isInt({ min: 1, max: 5 }),
    check('comment', 'Komentarz jest wymagany').not().isEmpty()
  ],
  addReview
);

module.exports = router;
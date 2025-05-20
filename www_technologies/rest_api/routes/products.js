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


// Get all products and create a new product
router
  .route('/')
  .get(getProducts)
  .post(
    protect,
    authorize('admin'),
    [
      check('name', 'Product name is required').not().isEmpty(),
      check('description', 'Product description is required').not().isEmpty(),
      check('price', 'Price must be a positive number').isNumeric({ min: 0 }),
      check('category', 'Category is required').not().isEmpty(),
      check('stock', 'Stock must be a positive number').isNumeric({ min: 0 })
    ],
    createProduct
  );

// Get, update, and delete a single product
router
  .route('/:id')
  .get(getProduct)
  .put(
    protect,
    authorize('admin'),
    [
      check('name', 'Product name is required').optional().not().isEmpty(),
      check('description', 'Product description is required').optional().not().isEmpty(),
      check('price', 'Price must be a positive number').optional().isNumeric({ min: 0 }),
      check('category', 'Category is required').optional().not().isEmpty(),
      check('stock', 'Stock must be a positive number').optional().isNumeric({ min: 0 })
    ],
    updateProduct
  )
  .delete(protect, authorize('admin'), deleteProduct);

// Add a review to a product
router.post(
  '/:id/reviews',
  protect,
  [
    check('rating', 'Rating must be a number between 1 and 5').isInt({ min: 1, max: 5 }),
    check('comment', 'Comment is required').not().isEmpty()
  ],
  addReview
);

module.exports = router;
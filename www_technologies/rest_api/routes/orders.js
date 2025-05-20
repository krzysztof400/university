// routes/orders.js
const express = require('express');
const { check } = require('express-validator');
const { 
  getOrders, 
  getMyOrders,
  getOrder, 
  createOrder, 
  updateOrderStatus, 
  cancelOrder 
} = require('../controllers/orders');

const { protect, authorize } = require('../middleware/auth');

const router = express.Router();

// All routes require authentication
router.use(protect);

// Get all orders (admin only)
router.get('/', authorize('admin'), getOrders);

// Get orders of the logged-in user
router.get('/myorders', getMyOrders);

// Create a new order
router.post(
  '/',
  [
    check('items', 'Products are required').isArray({ min: 1 }),
    check('items.*.product', 'Product ID is required').not().isEmpty(),
    check('items.*.quantity', 'Quantity must be a positive number').isInt({ min: 1 }),
    check('shippingAddress.street', 'Street is required').not().isEmpty(),
    check('shippingAddress.city', 'City is required').not().isEmpty(),
    check('shippingAddress.postalCode', 'Postal code is required').not().isEmpty(),
    check('shippingAddress.country', 'Country is required').not().isEmpty(),
    check('paymentMethod', 'Payment method is required').not().isEmpty()
  ],
  createOrder
);

// Get a single order, update its status, and cancel the order
router
  .route('/:id')
  .get(getOrder)
  .put(
    authorize('admin'),
    [
      check('orderStatus', 'Order status is required').optional().isIn([
        'Accepted',
        'Processing',
        'Shipped',
        'Delivered',
        'Cancelled'
      ]),
      check('paymentStatus', 'Payment status is required').optional().isIn([
        'Pending',
        'Paid',
        'Cancelled'
      ])
    ],
    updateOrderStatus
  )
  .delete(cancelOrder);

module.exports = router;
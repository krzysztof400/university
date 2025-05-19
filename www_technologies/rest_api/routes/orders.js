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

// Wszystkie trasy potrzebują uwierzytelnienia
router.use(protect);

// Pobierz wszystkie zamówienia (tylko admin)
router.get('/', authorize('admin'), getOrders);

// Pobierz zamówienia zalogowanego użytkownika
router.get('/myorders', getMyOrders);

// Utwórz nowe zamówienie
router.post(
  '/',
  [
    check('items', 'Produkty są wymagane').isArray({ min: 1 }),
    check('items.*.product', 'ID produktu jest wymagane').not().isEmpty(),
    check('items.*.quantity', 'Ilość musi być liczbą dodatnią').isInt({ min: 1 }),
    check('shippingAddress.street', 'Ulica jest wymagana').not().isEmpty(),
    check('shippingAddress.city', 'Miasto jest wymagane').not().isEmpty(),
    check('shippingAddress.postalCode', 'Kod pocztowy jest wymagany').not().isEmpty(),
    check('shippingAddress.country', 'Kraj jest wymagany').not().isEmpty(),
    check('paymentMethod', 'Metoda płatności jest wymagana').not().isEmpty()
  ],
  createOrder
);

// Pobierz pojedyncze zamówienie, zaktualizuj status i anuluj zamówienie
router
  .route('/:id')
  .get(getOrder)
  .put(
    authorize('admin'),
    [
      check('orderStatus', 'Status zamówienia jest wymagany').optional().isIn([
        'Przyjęte',
        'W realizacji',
        'Wysłane',
        'Dostarczone',
        'Anulowane'
      ]),
      check('paymentStatus', 'Status płatności jest wymagany').optional().isIn([
        'Oczekująca',
        'Opłacona',
        'Anulowana'
      ])
    ],
    updateOrderStatus
  )
  .delete(cancelOrder);

module.exports = router;
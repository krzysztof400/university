// controllers/orders.js
const Order = require('../models/Order');
const Product = require('../models/Product');
const { validationResult } = require('express-validator');

// @desc    Pobierz wszystkie zamówienia
// @route   GET /api/orders
// @access  Private/Admin
exports.getOrders = async (req, res, next) => {
  try {
    const orders = await Order.find()
      .populate('user', 'name email')
      .populate('items.product', 'name price');

    res.status(200).json({
      success: true,
      count: orders.length,
      data: orders
    });
  } catch (err) {
    next(err);
  }
};

// @desc    Pobierz zamówienia zalogowanego użytkownika
// @route   GET /api/orders/myorders
// @access  Private
exports.getMyOrders = async (req, res, next) => {
  try {
    const orders = await Order.find({ user: req.user.id })
      .populate('items.product', 'name price');

    res.status(200).json({
      success: true,
      count: orders.length,
      data: orders
    });
  } catch (err) {
    next(err);
  }
};

// @desc    Pobierz pojedyncze zamówienie
// @route   GET /api/orders/:id
// @access  Private
exports.getOrder = async (req, res, next) => {
  try {
    const order = await Order.findById(req.params.id)
      .populate('user', 'name email')
      .populate('items.product', 'name price');

    if (!order) {
      return res.status(404).json({
        success: false,
        message: 'Zamówienie nie zostało znalezione'
      });
    }

    // Sprawdzenie czy zamówienie należy do zalogowanego użytkownika lub jest adminem
    if (order.user._id.toString() !== req.user.id && req.user.role !== 'admin') {
      return res.status(403).json({
        success: false,
        message: 'Brak uprawnień do wyświetlenia tego zamówienia'
      });
    }

    res.status(200).json({
      success: true,
      data: order
    });
  } catch (err) {
    next(err);
  }
};

// @desc    Utwórz nowe zamówienie
// @route   POST /api/orders
// @access  Private
exports.createOrder = async (req, res, next) => {
  try {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      return res.status(400).json({ errors: errors.array() });
    }

    const { items, shippingAddress, paymentMethod } = req.body;

    if (!items || items.length === 0) {
      return res.status(400).json({
        success: false,
        message: 'Brak produktów w zamówieniu'
      });
    }

    // Pobranie szczegółów produktów i obliczenie całkowitej ceny
    const orderItems = [];
    let totalPrice = 0;

    for (const item of items) {
      const product = await Product.findById(item.product);

      if (!product) {
        return res.status(404).json({
          success: false,
          message: `Produkt o ID ${item.product} nie został znaleziony`
        });
      }

      if (product.stock < item.quantity) {
        return res.status(400).json({
          success: false,
          message: `Niewystarczająca ilość produktu ${product.name}`
        });
      }

      // Aktualizacja stanu magazynowego
      product.stock -= item.quantity;
      await product.save();

      // Dodanie szczegółów produktu do zamówienia
      orderItems.push({
        product: product._id,
        quantity: item.quantity,
        price: product.price
      });

      // Aktualizacja całkowitej ceny
      totalPrice += product.price * item.quantity;
    }

    // Utworzenie zamówienia
    const order = await Order.create({
      user: req.user.id,
      items: orderItems,
      shippingAddress,
      paymentMethod,
      totalPrice
    });

    res.status(201).json({
      success: true,
      data: order
    });
  } catch (err) {
    next(err);
  }
};

// @desc    Aktualizuj status zamówienia
// @route   PUT /api/orders/:id
// @access  Private/Admin
exports.updateOrderStatus = async (req, res, next) => {
  try {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      return res.status(400).json({ errors: errors.array() });
    }

    const { orderStatus, paymentStatus } = req.body;

    let order = await Order.findById(req.params.id);

    if (!order) {
      return res.status(404).json({
        success: false,
        message: 'Zamówienie nie zostało znalezione'
      });
    }

    // Aktualizacja statusów
    if (orderStatus) order.orderStatus = orderStatus;
    if (paymentStatus) order.paymentStatus = paymentStatus;

    await order.save();

    res.status(200).json({
      success: true,
      data: order
    });
  } catch (err) {
    next(err);
  }
};

// @desc    Anuluj zamówienie
// @route   DELETE /api/orders/:id
// @access  Private
exports.cancelOrder = async (req, res, next) => {
  try {
    const order = await Order.findById(req.params.id);

    if (!order) {
      return res.status(404).json({
        success: false,
        message: 'Zamówienie nie zostało znalezione'
      });
    }

    // Sprawdzenie czy zamówienie należy do zalogowanego użytkownika lub jest adminem
    if (order.user.toString() !== req.user.id && req.user.role !== 'admin') {
      return res.status(403).json({
        success: false,
        message: 'Brak uprawnień do anulowania tego zamówienia'
      });
    }

    // Sprawdzenie czy zamówienie można anulować
    if (order.orderStatus === 'Wysłane' || order.orderStatus === 'Dostarczone') {
      return res.status(400).json({
        success: false,
        message: 'Nie można anulować zamówienia, które zostało już wysłane lub dostarczone'
      });
    }

    // Przywrócenie stanu magazynowego
    for (const item of order.items) {
      const product = await Product.findById(item.product);
      if (product) {
        product.stock += item.quantity;
        await product.save();
      }
    }

    // Aktualizacja statusów
    order.orderStatus = 'Anulowane';
    order.paymentStatus = 'Anulowana';
    await order.save();

    res.status(200).json({
      success: true,
      data: order
    });
  } catch (err) {
    next(err);
  }
};
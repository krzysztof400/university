// routes/users.js
const express = require('express');
const { check } = require('express-validator');
const { 
  getUsers, 
  getUser, 
  createUser, 
  updateUser, 
  deleteUser 
} = require('../controllers/users');

const { protect, authorize } = require('../middleware/auth');

const router = express.Router();

// All routes require authentication and admin authorization
router.use(protect);
router.use(authorize('admin'));

// Get all users and create a new user
router
  .route('/')
  .get(getUsers)
  .post(
    [
      check('name', 'Name is required').not().isEmpty(),
      check('email', 'Please provide a valid email address').isEmail(),
      check('password', 'Password must be at least 6 characters long').isLength({ min: 6 })
    ],
    createUser
  );

// Get, update, and delete a single user
router
  .route('/:id')
  .get(getUser)
  .put(
    [
      check('name', 'Name is required').optional().not().isEmpty(),
      check('email', 'Please provide a valid email address').optional().isEmail(),
      check('password', 'Password must be at least 6 characters long').optional().isLength({ min: 6 })
    ],
    updateUser
  )
  .delete(deleteUser);

module.exports = router;
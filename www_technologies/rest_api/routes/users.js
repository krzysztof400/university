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

// Wszystkie trasy potrzebują uwierzytelnienia i uprawnienia administratora
router.use(protect);
router.use(authorize('admin'));

// Pobierz wszystkich użytkowników i utwórz nowego użytkownika
router
  .route('/')
  .get(getUsers)
  .post(
    [
      check('name', 'Imię i nazwisko jest wymagane').not().isEmpty(),
      check('email', 'Proszę podać prawidłowy adres email').isEmail(),
      check('password', 'Hasło musi mieć co najmniej 6 znaków').isLength({ min: 6 })
    ],
    createUser
  );

// Pobierz, zaktualizuj i usuń pojedynczego użytkownika
router
  .route('/:id')
  .get(getUser)
  .put(
    [
      check('name', 'Imię i nazwisko jest wymagane').optional().not().isEmpty(),
      check('email', 'Proszę podać prawidłowy adres email').optional().isEmail(),
      check('password', 'Hasło musi mieć co najmniej 6 znaków').optional().isLength({ min: 6 })
    ],
    updateUser
  )
  .delete(deleteUser);

module.exports = router;
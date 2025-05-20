const fetch = require('node-fetch');
const dotenv = require('dotenv');
const chalk = require('chalk');
const crypto = require('crypto');
dotenv.config();

// Configuration
const BASE_URL = `http://localhost:${process.env.PORT || 3000}/api`;

// Store tokens and IDs for later use
let userToken = '';
let adminToken = '';
let productId = '';
let secondProductId = '';


function logTest(name, status, data) {
  const statusColor = status >= 200 && status < 300 ? chalk.green : chalk.red;
  
  console.log(`\n--- TEST: ${chalk.bold(name)} ---`);
  console.log(`Status: ${statusColor(status)}`);
  
  if (data) {
    if (typeof data === 'string') {
      console.log('Response:', data);
    } else {
      console.log('Response:', JSON.stringify(data, null, 2));
    }
  }
}

// ============================
// Authentication Test Functions
// ============================

/**
 * Register a new user
 * @param {Object} userData - User data for registration
 * @returns {Promise<Object>} Registration response
 */
async function registerUser(userData = null) {
  const defaultUser = {
    name: 'Jan Kowalski',
    email: 'jan@example.com',
    password: 'password123',
    address: {
      street: 'ul. Przykładowa 1',
      city: 'Warszawa',
      postalCode: '00-001',
      country: 'Polska'
    }
  };

  const user = userData || defaultUser;
  
  const res = await fetch(`${BASE_URL}/auth/register`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(user)
  });

  const data = await res.json();
  logTest('Register User', res.status, data);
  return data;
}

/**
 * Register a second test user
 * @returns {Promise<Object>} Registration response
 */
async function registerSecondUser() {
  return registerUser({
    name: 'Arta Kserskes',
    email: 'persiangoat@example.com',
    password: 'password123',
    address: {
      street: 'ul. Przykładowa 2',
      city: 'Warszawa',
      postalCode: '00-002',
      country: 'Polska'
    }
  });
}

/**
 * Login user with credentials
 * @param {Object} credentials - Login credentials
 * @returns {Promise<string>} Authentication token
 */
async function loginUser(credentials = null) {
  const defaultCredentials = {
    email: 'jan@example.com',
    password: 'password123'
  };

  const creds = credentials || defaultCredentials;
  
  const res = await fetch(`${BASE_URL}/auth/login`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(creds)
  });

  const data = await res.json();
  
  if (res.ok && data.token) {
    userToken = data.token;
    logTest('Login User', res.status, { message: 'Login successful', token: data.token.substring(0, 15) + '...' });
    return data.token;
  } else {
    logTest('Login User', res.status, data);
    return null;
  }
}

/**
 * Login as second user
 * @returns {Promise<string>} Authentication token
 */
async function loginSecondUser() {
  return loginUser({
    email: 'persiangoat@example.com',
    password: 'password123'
  });
}

/**
 * Login as admin
 * @returns {Promise<string>} Admin authentication token
 */
async function loginAdmin() {
  const res = await fetch(`${BASE_URL}/auth/login`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      email: 'adminmail@mail.com',
      password: 'adminpass',
    })
  });

  const data = await res.json();
  
  if (res.ok && data.token) {
    adminToken = data.token;
    logTest('Login Admin', res.status, { message: 'Admin login successful', token: data.token.substring(0, 15) + '...' });
    return data.token;
  } else {
    logTest('Login Admin', res.status, data);
    return null;
  }
}

/**
 * Get current user profile
 * @param {string} token - Authentication token
 * @returns {Promise<Object>} User profile data
 */
async function getMe(token) {
  const useToken = token || userToken;
  
  const res = await fetch(`${BASE_URL}/auth/me`, {
    method: 'GET',
    headers: { Authorization: `Bearer ${useToken}` }
  });

  const data = await res.json();
  logTest('Get User Profile', res.status, data);
  return data;
}

/**
 * Logout user
 * @param {string} token - Authentication token
 * @returns {Promise<Object>} Logout response
 */
async function logoutUser(token) {
  const useToken = token || userToken;
  
  const res = await fetch(`${BASE_URL}/auth/logout`, {
    method: 'GET',
    headers: { Authorization: `Bearer ${useToken}` }
  });

  const data = await res.json();
  logTest('Logout User', res.status, data);
  return data;
}

/**
 * Test authentication with invalid token
 * @returns {Promise<Object>} Response data
 */
async function testInvalidToken() {
  const res = await fetch(`${BASE_URL}/auth/me`, {
    headers: { Authorization: 'Bearer invalidtoken123' }
  });

  const data = await res.json();
  logTest('Invalid Token Test', res.status, data);
  return data;
}

// ============================
// Product Test Functions
// ============================

async function addProduct(productData, token) {
    const useToken = token || adminToken;
    const res = await fetch(`${BASE_URL}/products`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${useToken}`
      },
      body: JSON.stringify(productData)
    });
  
    const data = await res.json();
    console.log('addProduct response status:', res.status);
    console.log('addProduct response body:', data);
    logTest('Add Product', res.status, data);
  
    // Attempt to extract ID from various possible locations
    const id =
      data.data?._id || data.data?.id ||
      data.product?._id || data.product?.id ||
      data._id || data.id ||
      null;
  
    if (!id) {
      throw new Error(
        'addProduct returned no ID. Ensure required fields match API schema. Response logged above.'
      );
    }
    return id;
  }

/**
 * Get all products
 * @returns {Promise<Array>} List of products
 */
async function getProducts() {
    const res = await fetch(`${BASE_URL}/products`);
    const responseData = await res.json();
    
    let data;
    if (Array.isArray(responseData)) {
      data = responseData;  // API returns an array directly
    } else if (responseData && Array.isArray(responseData.data)) {
      data = responseData.data;  // API returns an object with a 'data' array
    } else {
      logTest('Get All Products', res.status, { error: 'Unexpected response format' });
      return;
    }
    
    logTest('Get All Products', res.status, { count: data.length, products: data.slice(0, 2) });
    return data;
  }

/**
 * Get product by ID
 * @param {string} id - Product ID
 * @returns {Promise<Object>} Product data
 */
async function getProductById(id) {
  const res = await fetch(`${BASE_URL}/products/${id}`);
  const data = await res.json();
  
  logTest('Get Product by ID', res.status, data);
  return data;
}

/**
 * Update product
 * @param {string} id - Product ID
 * @param {Object} updates - Fields to update
 * @param {string} token - Authentication token
 * @returns {Promise<Object>} Updated product data
 */
async function updateProduct(id, updates, token) {
  const useToken = token || adminToken;
  
  const res = await fetch(`${BASE_URL}/products/${id}`, {
    method: 'PUT',
    headers: {
      'Content-Type': 'application/json',
      Authorization: `Bearer ${useToken}`
    },
    body: JSON.stringify(updates)
  });

  const data = await res.json();
  logTest('Update Product', res.status, data);
  return data;
}

/**
 * Delete product
 * @param {string} id - Product ID
 * @param {string} token - Authentication token
 * @returns {Promise<number>} HTTP status code
 */
async function deleteProduct(id, token) {
  const useToken = token || adminToken;
  
  const res = await fetch(`${BASE_URL}/products/${id}`, {
    method: 'DELETE',
    headers: {
      Authorization: `Bearer ${useToken}`
    }
  });

  const responseText = await res.text();
  logTest('Delete Product', res.status, responseText);
  return res.status;
}

/**
 * Add product review
 * @param {string} productId - Product ID
 * @param {Object} reviewData - Review data
 * @param {string} token - Authentication token
 * @returns {Promise<Object>} Review response data
 */
async function addProductReview(productId, reviewData, token) {
  const useToken = token || userToken;
  
  const res = await fetch(`${BASE_URL}/products/${productId}/reviews`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      Authorization: `Bearer ${useToken}`
    },
    body: JSON.stringify(reviewData)
  });

  const data = await res.json();
  logTest('Add Product Review', res.status, data);
  return data;
}

/**
 * Test invalid product creation (missing fields)
 * @param {string} token - Authentication token
 * @returns {Promise<Object>} Response data
 */
async function testInvalidProductCreation(token) {
  const useToken = token || adminToken;
  
  const res = await fetch(`${BASE_URL}/products`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      Authorization: `Bearer ${useToken}`
    },
    body: JSON.stringify({
      name: 'Invalid Product',
      // Missing required fields: description, price, category, stock
    })
  });

  const data = await res.json();
  logTest('Create Invalid Product', res.status, data);
  return data;
}

/**
 * Test unauthorized product creation (as regular user)
 * @returns {Promise<Object>} Response data
 */
async function testUnauthorizedProductCreation() {
  // Ensure we're using user token, not admin token
  const res = await fetch(`${BASE_URL}/products`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      Authorization: `Bearer ${userToken}`
    },
    body: JSON.stringify({
      name: 'Smartphone ABC',
      description: 'Latest smartphone model',
      price: 2499.99,
      category: 'Elektronika',
      stock: 15
    })
  });

  const data = await res.json();
  logTest('Unauthorized Product Creation', res.status, data);
  return data;
}

/**
 * Test product filtering and sorting
 * @returns {Promise<Array>} Filtered product list
 */
async function testProductFiltering() {
    // Make the API request
    const res = await fetch(`${BASE_URL}/products?category=Elektronika&sort=price`);
    
    // Check if the response is successful
    if (!res.ok) {
      const errorText = await res.text();
      logTest('Filter Products by Category and Sort by Price', res.status, { error: errorText });
      return;
    }
    
    // Parse the JSON response
    const responseData = await res.json();
    
    // Determine the array of products
    let data;
    if (Array.isArray(responseData)) {
      data = responseData; // API returns an array directly
    } else if (responseData && Array.isArray(responseData.data)) {
      data = responseData.data; // API returns an object with a 'data' array
    } else {
      logTest('Filter Products by Category and Sort by Price', res.status, { error: 'Unexpected response format' });
      return;
    }
    
    // Log the test results using the array
    logTest('Filter Products by Category and Sort by Price', res.status, {
      count: data.length,
      products: data.slice(0, 2)
    });
    
    return data;
  }
// ============================
// Order Test Functions
// ============================

/**
 * Create a new order
 * @param {Object} orderData - Order data
 * @param {string} token - Authentication token
 * @returns {Promise<Object>} Order response data
 */
async function createOrder(orderData, token) {
    const res = await fetch(`${BASE_URL}/orders`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${token}`
      },
      body: JSON.stringify(orderData)
    });
    const data = await res.json();
    logTest('Create Order', res.status, data);
    return data;
  }
  
  /**
   * Get user's orders
   * @param {string} token - Authentication token
   * @returns {Promise<Array>} List of user's orders
   */
  async function getMyOrders(token) {
    const res = await fetch(`${BASE_URL}/orders/myorders`, {
      method: 'GET',
      headers: {
        Authorization: `Bearer ${token}`
      }
    });
    const data = await res.json();
    logTest('Get My Orders', res.status, data);
    return data;
  }
  
  /**
   * Get all orders (admin only)
   * @param {string} token - Authentication token
   * @returns {Promise<Array>} List of all orders
   */
  async function getAllOrders(token) {
    const res = await fetch(`${BASE_URL}/orders`, {
      method: 'GET',
      headers: {
        Authorization: `Bearer ${token}`
      }
    });
    const data = await res.json();
    logTest('Get All Orders', res.status, data);
    return data;
  }
  
  /**
   * Get order by ID
   * @param {string} orderId - Order ID
   * @param {string} token - Authentication token
   * @returns {Promise<Object>} Order data
   */
  async function getOrderById(orderId, token) {
    const res = await fetch(`${BASE_URL}/orders/${orderId}`, {
      method: 'GET',
      headers: {
        Authorization: `Bearer ${token}`
      }
    });
    const data = await res.json();
    logTest('Get Order by ID', res.status, data);
    return data;
  }
  
  /**
   * Update order status (admin only)
   * @param {string} orderId - Order ID
   * @param {Object} updates - Status updates
   * @param {string} token - Authentication token
   * @returns {Promise<Object>} Updated order data
   */
  async function updateOrderStatus(orderId, updates, token) {
    const res = await fetch(`${BASE_URL}/orders/${orderId}`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${token}`
      },
      body: JSON.stringify(updates)
    });
    const data = await res.json();
    logTest('Update Order Status', res.status, data);
    return data;
  }
  
  /**
   * Cancel order
   * @param {string} orderId - Order ID
   * @param {string} token - Authentication token
   * @returns {Promise<Object>} Cancellation response
   */
  async function cancelOrder(orderId, token) {
    const res = await fetch(`${BASE_URL}/orders/${orderId}`, {
      method: 'DELETE',
      headers: {
        Authorization: `Bearer ${token}`
      }
    });
    const data = await res.json();
    logTest('Cancel Order', res.status, data);
    return data;
  }
  
  /**
   * Test creating an invalid order
   * @param {Object} invalidOrderData - Invalid order data
   * @param {string} token - Authentication token
   * @returns {Promise<Object>} Response data
   */
  async function testInvalidOrderCreation(invalidOrderData, token) {
    const res = await fetch(`${BASE_URL}/orders`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${token}`
      },
      body: JSON.stringify(invalidOrderData)
    });
    const data = await res.json();
    logTest('Create Invalid Order', res.status, data);
    return data;
  }
  
  async function runOrderTests() {
    try {
      console.log(chalk.bgBlue.white('\n========================='));
      console.log(chalk.bgBlue.white('STARTING ORDER TESTS'));
      console.log(chalk.bgBlue.white('========================='));
  
      // Login as admin to create a test product
      await loginAdmin();
      // Adjusted payload: use 'stock' field instead of 'countInStock'
      const productData = {
        name: 'Test Product',
        description: 'Product for order testing',
        price: 9.99,
        stock: 10,
        category: 'Elektronika'
      };
  
      // Create product using enhanced addProduct helper
      const productId = await addProduct(productData, adminToken);
      console.log('Created test product with ID:', productId);
  
      // Login as user
      await loginUser();
  
      // Create an order with valid data
      const orderData = {
        items: [{ product: productId, quantity: 2 }],
        shippingAddress: {
          street: 'Test Street',
          city: 'Test City',
          postalCode: '12345',
          country: 'Test Country'
        },
        paymentMethod: 'Karta'
      };
  
      let orderResponse;
      try {
        orderResponse = await createOrder(orderData, userToken);
      } catch (error) {
        console.error('createOrder failed:', error.response?.data || error);
        throw new Error('Failed to create order');
      }
  
      const orData = orderResponse?.data;
      if (!orData) {
        throw new Error('createOrder response is null or undefined');
      }
      console.log('Raw createOrder response data:', orData);
  
      const createdOrder = orData.order || orData;
      if (!createdOrder || !createdOrder._id) {
        throw new Error('Order ID not found in createOrder response: ' + JSON.stringify(orData));
      }
      const orderId = createdOrder._id;
      console.log('Created order with ID:', orderId);
  
      // Continue with the rest of the tests
      await getMyOrders(userToken);
      await getOrderById(orderId, userToken);
      await getAllOrders(adminToken);
      await updateOrderStatus(orderId, { orderStatus: 'W realizacji' }, adminToken);
      await loginUser();
      await updateOrderStatus(orderId, { orderStatus: 'W realizacji' }, userToken);
      await cancelOrder(orderId, userToken);
  
      // Test creating an order with invalid data
      const invalidOrderData = {
        items: [],
        shippingAddress: {
          street: 'Test StreetWithInvalidOrderData',
          city: 'Test City',
          postalCode: '12345',
          country: 'Test Country'
        },
        paymentMethod: 'Credit Card'
      };
      await testInvalidOrderCreation(invalidOrderData, userToken);
  
      console.log(chalk.bgGreen.black('\n========================='));
      console.log(chalk.bgGreen.black('ORDER TESTS COMPLETED'));
      console.log(chalk.bgGreen.black('========================='));
  
    } catch (err) {
      console.error(chalk.bgRed.white('\nORDER TEST FAILURE:'), err);
    }
  }
  
  
// ============================
// Test Suites
// ============================

/**
 * Run authentication tests
 */
async function runAuthTests() {
  try {
    console.log(chalk.bgBlue.white('\n========================='));
    console.log(chalk.bgBlue.white('STARTING AUTH TESTS'));
    console.log(chalk.bgBlue.white('=========================\n'));

    // Register users
    await registerUser();
    await registerSecondUser();
    
    // Login tests
    await loginUser();
    await getMe(userToken);
    
    // Invalid token test
    await testInvalidToken();
    
    // Logout test
    await logoutUser(userToken);
    
    // Login as second user
    await loginSecondUser();
    
    console.log(chalk.bgGreen.black('\n========================='));
    console.log(chalk.bgGreen.black('AUTH TESTS COMPLETED'));
    console.log(chalk.bgGreen.black('=========================\n'));
  } catch (err) {
    console.error(chalk.bgRed.white('\nAUTH TEST FAILURE:'), err);
  }
}

/**
 * Run product tests
 */
async function runProductTests() {
  try {
    console.log(chalk.bgBlue.white('\n========================='));
    console.log(chalk.bgBlue.white('STARTING PRODUCT TESTS'));
    console.log(chalk.bgBlue.white('=========================\n'));

    // Login as admin for product management
    await loginAdmin();
    
    // Add test products
    productId = await addProduct({
      name: 'Laptop XYZ',
      description: 'Wysokiej klasy laptop do pracy i rozrywki',
      price: 3499.99,
      category: 'Elektronika',
      stock: 10
    }, adminToken);
    
    secondProductId = await addProduct({
      name: 'Smartwatch PRO',
      description: 'Zaawansowany zegarek sportowy z GPS',
      price: 999.99,
      category: 'Elektronika',
      stock: 25
    }, adminToken);

    // Get product information
    await getProducts();
    await getProductById(productId);
    
    // Update product
    await updateProduct(productId, {
      price: 3299.99,
      stock: 15
    }, adminToken);
    
    // Verify the update
    await getProductById(productId);
    
    // Test invalid product creation
    await testInvalidProductCreation(adminToken);
    
    // Login as regular user
    await loginUser();
    
    // Test unauthorized product creation
    await testUnauthorizedProductCreation();
    
    // Test product filtering
    await testProductFiltering();
    
    // Add product reviews as regular user
    await addProductReview(productId, {
      rating: 5,
      comment: 'Doskonały produkt, polecam!'
    }, userToken);
    
    await addProductReview(productId, {
      rating: 3,
      comment: 'Przeciętny produkt za tę cenę'
    }, userToken);
    
    // Verify product with reviews
    await getProductById(productId);
    
    // Login back as admin for cleanup
    await loginAdmin();
    
    // Delete test products
    if (productId) {
      await deleteProduct(productId, adminToken);
    }
    
    if (secondProductId) {
      await deleteProduct(secondProductId, adminToken);
    }
    
    // Verify products are gone
    await getProducts();
    
    console.log(chalk.bgGreen.black('\n========================='));
    console.log(chalk.bgGreen.black('PRODUCT TESTS COMPLETED'));
    console.log(chalk.bgGreen.black('=========================\n'));
  } catch (err) {
    console.error(chalk.bgRed.white('\nPRODUCT TEST FAILURE:'), err);
  }
}

function modifyJwtHeader(token, newHeader, secret) {
  const [header, payload, signature] = token.split('.');

  // Decode the payload
  const decodedPayload = Buffer.from(payload, 'base64').toString('utf8');

  // Create new header
  const newHeaderBase64 = Buffer.from(JSON.stringify(newHeader)).toString('base64');

  // Re-sign the token with the new header and algorithm
  const newSignature = crypto
    .createHmac(newHeader.alg === 'HS256' ? 'sha256' : 'sha512', secret)
    .update(`${newHeaderBase64}.${payload}`)
    .digest('base64');

  // Return the modified token
  return `${newHeaderBase64}.${payload}.${newSignature}`;
}
/**
 * Main test runner function
 */
async function runAllTests() {
  console.log(chalk.bgMagenta.white('\n============================='));
  console.log(chalk.bgMagenta.white('API TEST SUITE'));
  console.log(chalk.bgMagenta.white(`BASE URL: ${BASE_URL}`));
  console.log(chalk.bgMagenta.white('=============================\n'));
  
  try {
    // Run auth tests first
    await runAuthTests();
    
    // Then run product tests
    await runProductTests();
    
    // Finally, run order tests
    await runOrderTests();

    console.log(chalk.bgGreen.black('\n============================='));
    console.log(chalk.bgGreen.black('ALL TESTS COMPLETED SUCCESSFULLY'));
    console.log(chalk.bgGreen.black('=============================\n'));
  } catch (err) {
    console.error(chalk.bgRed.white('\nTEST SUITE FAILURE:'), err);
  }
}

// Run all tests
runAllTests();
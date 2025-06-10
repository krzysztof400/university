import numpy as np

class TwoLayerNN:
    def __init__(self):
        # Weights and biases for the hidden layer (4 gates)
        self.W1 = np.random.randn(2, 4)  # 2 inputs to 4 hidden neurons
        self.b1 = np.random.randn(4)      # 4 biases for hidden neurons
        
        # Weights and biases for the output layer (1 gate)
        self.W2 = np.random.randn(4, 1)   # 4 hidden neurons to 1 output
        self.b2 = np.random.randn(1)      # 1 bias for output neuron

    def relu(self, x):
        return np.maximum(0, x)

    def forward(self, x):
        # L1 normalization
        x_normalized = self.l1_normalize(x)
        
        # Hidden layer
        z1 = np.dot(x_normalized, self.W1) + self.b1
        a1 = self.relu(z1)
        
        # Output layer
        z2 = np.dot(a1, self.W2) + self.b2
        output = self.sigmoid(z2)
        
        return output

    def l1_normalize(self, x):
        norm = np.sum(np.abs(x))
        return x / norm if norm != 0 else x

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))
    
    def predict(self, x):
        # Forward pass to get the prediction
        output = self.forward(x)
        if output >= 0.5:
            return 1
        else:
            return 0
        
    def train(self, X, y, epochs=1000, learning_rate=0.01):
        for epoch in range(epochs):
            # Forward pass
            z1 = np.dot(X, self.W1) + self.b1
            a1 = self.relu(z1)
            z2 = np.dot(a1, self.W2) + self.b2
            output = self.sigmoid(z2)

            # Compute loss (mean squared error)
            loss = np.mean((output - y) ** 2)

            # Backward pass (gradient descent)
            d_output = output - y
            d_z2 = d_output * (output * (1 - output))
            d_W2 = np.dot(a1.T, d_z2)
            d_b2 = np.sum(d_z2, axis=0)

            d_a1 = np.dot(d_z2, self.W2.T)
            d_z1 = d_a1 * (z1 > 0)
            d_W1 = np.dot(X.T, d_z1)
            d_b1 = np.sum(d_z1, axis=0)
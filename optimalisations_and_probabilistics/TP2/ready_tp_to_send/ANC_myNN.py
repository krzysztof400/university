import numpy as np

# Load EEG data
S = np.loadtxt('Data_EEG.txt', skiprows=1, unpack=True)
S_sqr = np.square(S)
Mean_S_sqr = np.mean(S_sqr)
n = len(S)

np.random.seed(42)

timesteps = n
fs = 180
f_noise = 60

# Generate noise signals
k = np.arange(timesteps)
noise = 1.2 * np.sin(2 * np.pi * k * 3 / fs)
filtered_noise = 0.12 * np.sin(2 * np.pi * k * 3 / fs + np.pi / 2)
target = S + filtered_noise

# Neural network training function
def fit_with_n_neuron_in_layer_neural_net(input_data, label, n_neurons, N, alpha=0.01, batch_size=3):
    def sigmoid(x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(x):
        return x * (1 - x)
    
    np.random.seed(42)  # Ensure reproducibility

    # Initialize weights and biases
    w1 = np.random.normal(size=(input_data.shape[1], n_neurons))
    b1 = np.random.normal(size=(1, n_neurons))
    w2 = np.random.normal(size=(n_neurons, 1))
    b2 = np.random.normal(size=(1, 1))

    loss = []
    w_list = []

    for i in range(N):
        # Select a random batch
        j = np.random.randint(0, len(input_data), batch_size)
        X_batch = input_data[j]
        y_batch = label[j].reshape(-1, 1)

        # Forward pass
        Z1 = sigmoid(np.dot(X_batch, w1) + b1)
        Z2 = sigmoid(np.dot(Z1, w2) + b2)

        # Compute loss (Mean Squared Error)
        error = np.mean((y_batch - Z2) ** 2)
        
        # Backpropagation
        dZ2 = 2 * (y_batch - Z2) * sigmoid_derivative(Z2)
        dW2 = np.dot(Z1.T, dZ2)
        db2 = np.sum(dZ2, axis=0, keepdims=True)

        dZ1 = np.dot(dZ2, w2.T) * sigmoid_derivative(Z1)
        dW1 = np.dot(X_batch.T, dZ1)
        db1 = np.sum(dZ1, axis=0, keepdims=True)

        # Update weights and biases
        w1 += alpha * dW1
        b1 += alpha * db1
        w2 += alpha * dW2
        b2 += alpha * db2

        # Store weight updates and loss
        w_list.append((w1.copy(), w2.copy()))
        loss.append(error)

        # Print loss every 1000 iterations
        if i % 1000 == 0:
            print(f"Iteration: {i}, Loss: {error:.6f}")

    return Z2, loss, w_list

X1 = noise.reshape(-1, 1)
X2 = np.column_stack([noise, np.roll(noise, 1)])
X3 = np.column_stack([noise, np.roll(noise, 1), np.roll(noise, 2)])

y = target

out1, loss1, w_list1 = fit_with_n_neuron_in_layer_neural_net(X1, y, 1, N=1001, alpha=0.01, batch_size=3)
out2, loss2, w_list2 = fit_with_n_neuron_in_layer_neural_net(X2, y, 1, N=1001, alpha=0.01, batch_size=3)
out3, loss3, w_list3 = fit_with_n_neuron_in_layer_neural_net(X3, y, 1, N=1001, alpha=0.01, batch_size=3)

# Print final results
print(f"Loss (1 neuron): {loss1[-1]}")
print("First-Order ADALINE Weights:", w_list1[-1])

print(f"Loss (2 neurons): {loss2[-1]}")
print("Second-Order ADALINE Weights:", w_list2[-1])

print(f"Loss (3 neurons): {loss3[-1]}")
print("Third-Order ADALINE Weights:", w_list3[-1])

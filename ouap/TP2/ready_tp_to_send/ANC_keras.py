import os
os.environ['CUDA_VISIBLE_DEVICES'] = '0'

import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import matplotlib.pyplot as plt
from ANC_myNN import fit_with_n_neuron_in_layer_neural_net

S = np.loadtxt('Data_EEG.txt', skiprows=1, unpack=True)
S_sqr = np.square(S)
Mean_S_sqr = np.mean(S_sqr)

np.random.seed(42)
timesteps = len(S)
fs = 180
f_noise = 60

k = np.arange(timesteps)
noise = 1.2 * np.sin(2 * np.pi * k * 3 / fs)
filtered_noise = 0.12 * np.sin(2 * np.pi * k * 3 / fs + np.pi / 2)
target = S + filtered_noise

X1 = noise.reshape(-1, 1)
y = filtered_noise.reshape(-1, 1)
model1 = keras.Sequential([
    layers.Dense(1, input_dim=1, activation='linear', use_bias=False)
])
model1.compile(optimizer=keras.optimizers.SGD(learning_rate=0.01), loss='mse')
model1.fit(X1, y, epochs=1000, verbose=0)
predicted_noise1 = model1.predict(X1)
filtered_signal1 = target - predicted_noise1.flatten()

X2 = np.column_stack([noise, np.roll(noise, 1)])
X2[0, 1] = 0 
model2 = keras.Sequential([
    layers.Dense(1, input_dim=2, activation='linear', use_bias=False)
])
model2.compile(optimizer=keras.optimizers.SGD(learning_rate=0.01), loss='mse')
model2.fit(X2, y, epochs=1000, verbose=0)
predicted_noise2 = model2.predict(X2)
filtered_signal2 = target - predicted_noise2.flatten()

X3 = np.column_stack([noise, np.roll(noise, 1), np.roll(noise, 2)])
X3[0, 1] = 0
X3[1, 2] = 0
model3 = keras.Sequential([
    layers.Dense(1, input_dim=3, activation='linear', use_bias=False)
])
model3.compile(optimizer=keras.optimizers.SGD(learning_rate=0.01), loss='mse')
model3.fit(X3, y, epochs=1000, verbose=0)
predicted_noise3 = model3.predict(X3)
filtered_signal3 = target - predicted_noise3.flatten()

print("USING KERAS")
print("First-Order ADALINE Weights:", model1.get_weights())
print("Second-Order ADALINE Weights:", model2.get_weights())
print("Third-Order ADALINE Weights:", model3.get_weights())

plt.figure(figsize=(10, 5))
plt.plot(S, label='Pure EEG Signal S(k)', color='blue')
plt.plot(target, label='Unfiltered Signal T(k)', color='red', alpha=0.6)
plt.legend()
plt.xlabel('Time Steps')
plt.ylabel('Signal Amplitude')
plt.title('Pure EEG Signal vs Unfiltered Signal')
plt.show()

print("USING MY OWN IMPLEMENTATION")
print(f"Loss (1 neuron): {loss1[-1]}")
print("First-Order ADALINE Weights:", w_list1[-1])
print(f"Loss (2 neurons): {loss2[-1]}")
print("Second-Order ADALINE Weights:", w_list2[-1])
print(f"Loss (3 neurons): {loss3[-1]}")
print("Third-Order ADALINE Weights:", w_list3[-1])
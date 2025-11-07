import numpy as np
import matplotlib.pyplot as plt
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam

# Generate dataset
X = np.random.randint(0, 2, (200, 2))
dataset = {
    "AND": {"X": X, "y": np.array([x[0] and x[1] for x in X], dtype=int)},
    "OR": {"X": X, "y": np.array([x[0] or x[1] for x in X], dtype=int)},
    "XOR": {"X": X, "y": np.array([(x[0] and not x[1]) or (not x[0] and x[1]) for x in X], dtype=int)}
}

def build_model():
    model = Sequential([
        Dense(4, input_dim=2, activation='sigmoid'),
        Dense(1, activation='sigmoid')
    ])
    model.compile(optimizer=Adam(learning_rate=0.01), loss='mse')
    return model

def train_and_plot(dataset):
    fig, ax = plt.subplots(len(dataset), 2, figsize=(10, 8))
    for i, (name, ds) in enumerate(dataset.items()):
        model = build_model()
        history = model.fit(ds["X"], ds["y"], epochs=100, verbose=0, batch_size=4, validation_split=0.1)
        
        predictions = model.predict(ds["X"])[:, 0]
        
        ax[i, 0].plot(predictions, label="Output")
        ax[i, 0].plot(ds["y"], label="Label", linestyle='dashed')
        ax[i, 0].set_title(f"Simulation of {name}")
        ax[i, 0].legend()
        
        ax[i, 1].plot(history.history['loss'], label="Loss")
        ax[i, 1].set_title(f"Loss of {name}")
        ax[i, 1].legend()
    
    plt.tight_layout()
    plt.show()

train_and_plot(dataset)

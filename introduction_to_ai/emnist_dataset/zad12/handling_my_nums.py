from tensorflow import keras
import tensorflow as tf
from PIL import Image
import os
import numpy as np
from PIL import ImageFilter


# Wczytanie modelu
model = keras.models.load_model('emnist_model.h5')

# Wczytanie własnych próbek
custom_images = []
custom_labels = []  # Etykiety 0-9

for filename in os.listdir('my_numbers'):
    if filename.endswith('.png'):
        img = Image.open(f'my_numbers/{filename}').convert('L')  # Skala szarości
        img = img.resize((28, 28))  # Przeskalowanie
        img = img.filter(ImageFilter.GaussianBlur(radius=1))  # Rozmycie z większym gradientem
        img_array = np.array(img) / 255.0  # Normalizacja
        img.save(f'my_numbers_processed/{filename}')
        custom_images.append(img_array.flatten())  # Spłaszczenie do 784
        custom_labels.append(len(custom_labels) // 9)

custom_images = np.array(custom_images)
custom_labels = np.array(custom_labels)

test_acc = model.evaluate(custom_images, custom_labels)
print(f"Dokładność na zbiorze testowym: {test_acc[1]:.4f}")
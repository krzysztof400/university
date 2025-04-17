import tensorflow as tf
import tensorflow_datasets as tfds
from tensorflow import keras
from tensorflow.keras import layers

# Wczytanie zbioru EMNIST Balanced
dataset_name = "emnist/balanced"
(ds_train, ds_test), ds_info = tfds.load(
    dataset_name,
    split=['train', 'test'],
    shuffle_files=True,
    as_supervised=True,
    with_info=True,
)

def filter_digits(image, label):
    return tf.math.less(label, 10)

def preprocess(image, label):
    image = tf.image.convert_image_dtype(image, tf.float32)
    image = tf.image.resize(image, (28, 28))
    image = tf.reshape(image, (784,))
    return image, label

batch_size = 64
ds_train = ds_train.filter(filter_digits).map(preprocess).shuffle(10000).batch(batch_size).prefetch(tf.data.AUTOTUNE)
ds_test = ds_test.filter(filter_digits).map(preprocess).batch(batch_size).prefetch(tf.data.AUTOTUNE)

# Budowa modelu
model = keras.Sequential([
    layers.Input(shape=(784,)),          # Wejście: spłaszczony obraz 28x28
    layers.Dense(128, activation='relu'), # Jedna warstwa ukryta
    layers.Dense(10, activation='softmax') # Wyjście: 10 klas (0-9)
])

# Kompilacja modelu
model.compile(
    optimizer='adam',
    loss='sparse_categorical_crossentropy',  # Etykiety nie są one-hot
    metrics=['accuracy']
)

# Trening modelu
model.fit(ds_train, epochs=5, verbose=1)

model.save('emnist_model.h5')
tf.data.experimental.save(ds_train, 'ds_train')
tf.data.experimental.save(ds_test, 'ds_test')
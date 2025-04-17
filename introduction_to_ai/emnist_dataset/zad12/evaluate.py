import tensorflow as tf
import tensorflow_datasets as tfds
from tensorflow import keras
from tensorflow.keras import layers

model = keras.models.load_model('emnist_model.h5')
ds_test = tf.data.experimental.load('ds_test')

test_acc = model.evaluate(ds_test)
print(f"Dokładność na zbiorze testowym: {test_acc[1]:.4f}")
import tensorflow as tf
from keras import models
from keras.api.layers import Conv2D, BatchNormalization, Dropout, MaxPooling2D, Dense, GlobalAveragePooling2D
from keras.api.utils import to_categorical
from keras.datasets import cifar10
from tensorflow.keras.models import load_model

# load dataset
(X_train, y_train), (X_test, y_test) = cifar10.load_data()

# normalize data: [0, 255] -> [0, 1]
X_train = X_train / 255
X_test = X_test / 255

# transform to one-hot representation
y_train = to_categorical(y_train, 10)
y_test = to_categorical(y_test, 10)

model = load_model("model.h5")

score = model.evaluate(X_test, y_test)
print('Test score:', score[0])
print('Test accuracy:', score[1])

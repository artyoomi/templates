import sys
from keras import models
from keras.api.layers import Conv2D, BatchNormalization, Dropout, MaxPooling2D, Dense, GlobalAveragePooling2D
from keras.api.utils import to_categorical
from keras.datasets import cifar10

def train_cifar_cnn(train_data, test_data):
    (X_train, y_train), (X_test, y_test) = train_data, test_data

    # Normalize data: [0, 255] -> [0, 1]
    X_train = X_train / 255
    X_test = X_test / 255

    # Transform to one-hot representation
    y_train = to_categorical(y_train, 10)
    y_test = to_categorical(y_test, 10)

    model = models.Sequential()

    # 32x32
    model.add(Conv2D(32, (3, 3), activation='relu',
                     input_shape=(32, 32, 3), padding='same'))
    model.add(BatchNormalization())
    model.add(Conv2D(32, (3, 3), activation='relu', padding='same'))
    model.add(BatchNormalization())
    model.add(MaxPooling2D((2, 2)))
    model.add(Dropout(0.2))

    # 16x16
    model.add(Conv2D(64, (3, 3), activation='relu', padding='same'))
    model.add(BatchNormalization())
    model.add(Conv2D(64, (3, 3), activation='relu', padding='same'))
    model.add(BatchNormalization())
    model.add(MaxPooling2D((2, 2)))
    model.add(Dropout(0.2))

    # 8x8
    model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(BatchNormalization())
    model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(BatchNormalization())
    model.add(MaxPooling2D((2, 2)))
    model.add(Dropout(0.2))

    # 4x4
    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(BatchNormalization())
    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(BatchNormalization())
    model.add(MaxPooling2D((2, 2)))
    model.add(Dropout(0.2))

    # transform 4x4x3 tensor to one dimensinal vector
    model.add(GlobalAveragePooling2D())

    model.add(Dense(512, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(10, activation='softmax'))

    model.summary()
    model.compile(loss='categorical_crossentropy', optimizer='adam',
                  metrics=['accuracy'])
    model.fit(X_train, y_train, batch_size=64, epochs=20, verbose=1)

    score = model.evaluate(X_test, y_test)
    print('Test score:', score[0])
    print('Test accuracy:', score[1])

    return model

if __name__ == "__main__":
    if (len(sys.argv) != 2):
        print(f"Usage: {sys.argv[0]} <MODEL_NAME>")
        exit(0)

    train_data, test_data = cifar10.load_data()
    model = train_cifar_cnn(train_data, test_data)
    model.save(sys.argv[1])

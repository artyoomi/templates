import numpy as np
import keras
import tensorflow as tf
from keras import layers, Model
import matplotlib.pyplot as plt

def visualize_reconstructions(model, test_data, num_images=10):
    """Visualize difference between image and reconstructed image."""
    
    # Select random image from test set
    indices = np.random.choice(len(test_data), size=num_images, replace=False)
    test_samples = test_data[indices]
    
    # Get reconstruction through the encoder
    reconstructed = model.predict(test_samples)
    
    # Configuring image display
    plt.figure(figsize=(20, 6))
    
    # Displaying original and reconstructions
    for i in range(num_images):
        # Original image
        ax = plt.subplot(2, num_images, i + 1)
        plt.imshow(test_samples[i])
        plt.title(f"Original_{i}")
        plt.axis("off")

        # Reconstruction
        ax = plt.subplot(2, num_images, i + 1 + num_images)
        plt.imshow(reconstructed[i])
        plt.title(f"Reconstructed_{i}")
        plt.axis("off")
    
    plt.tight_layout()
    plt.show()


@keras.saving.register_keras_serializable(package="CustomVAE", name="Sampling")
class Sampling(layers.Layer):
    """Use mu and log_var to sample z"""
    
    def call(self, inputs):
        """Calculate vector in latent space."""
        
        mu, log_var = inputs
        epsilon = tf.random.normal(shape=tf.shape(mu))
        return mu + tf.exp(log_var * 0.5) * epsilon


def create_cifar10_encoder(latent_dim):
    encoder_input = layers.Input(shape=(32, 32, 3))
    x = layers.Conv2D(filters=64,
                      kernel_size=4,
                      strides=(2, 2),
                      padding="same",
                      activation="relu",
                      name="conv1")(encoder_input)
    x = layers.BatchNormalization(name="batchnorm1")(x)
    x = layers.Conv2D(filters=128,
                      kernel_size=4,
                      strides=(2, 2),
                      padding="same",
                      activation="relu",
                      name="conv2")(x)
    x = layers.BatchNormalization(name="batchnorm2")(x)
    x = layers.Conv2D(filters=256,
                      kernel_size=4,
                      strides=(2, 2),
                      padding="same",
                      activation="relu",
                      name="conv3")(x)
    x = layers.BatchNormalization(name="batchnorm3")(x)
    x = layers.Conv2D(filters=512,
                      kernel_size=4,
                      strides=(2, 2),
                      padding="same",
                      activation="relu",
                      name="conv4")(x)
    x = layers.BatchNormalization(name="batchnorm4")(x)
    x = layers.GlobalMaxPooling2D(name="global_max_pooling2d")(x)
    
    mu_output = layers.Dense(latent_dim, name="mu")(x)
    log_var_output = layers.Dense(latent_dim, name="log_var")(x)
    z = Sampling()([mu_output, log_var_output])

    return keras.Model(encoder_input,
                       [mu_output, log_var_output, z],
                       name="encoder")
    

def create_cifar10_decoder(latent_dim):
    decoder_input = layers.Input(shape=(latent_dim,))
    x = layers.Dense(2 * 2 * 512)(decoder_input)
    x = layers.Reshape(target_shape=(2, 2, 512))(x)
    x = layers.Conv2DTranspose(filters=256,
                               kernel_size=(4, 4),
                               strides=(2, 2),
                               padding="same",
                               activation="relu",
                               name="deconv1")(x)
    x = layers.BatchNormalization(name="batchnorm1")(x)
    x = layers.Conv2DTranspose(filters=128,
                               kernel_size=(4, 4),
                               strides=(2, 2),
                               padding="same",
                               activation="relu",
                               name="deconv2")(x)
    x = layers.BatchNormalization(name="batchnorm2")(x)
    x = layers.Conv2DTranspose(filters=64,
                               kernel_size=(4, 4),
                               strides=(2, 2),
                               padding="same",
                               activation="relu",
                               name="deconv3")(x)
    x = layers.BatchNormalization(name="batchnorm3")(x)
    x = layers.Conv2DTranspose(filters=3,
                               kernel_size=(4, 4),
                               strides=(2, 2),
                               padding="same",
                               activation="sigmoid",
                               name="deconv4")(x)
    decoder_output = layers.BatchNormalization(name="batchnorm4")(x)
    
    return keras.Model(decoder_input, decoder_output, name="decoder")


@keras.saving.register_keras_serializable(package="CustomVAE", name="VAE")
class VAE(Model):
    """Combines encoder and decoder into end-to-end model for training."""
    
    def __init__(
        self,
        latent_dim=256,
        beta=0.1,
        name="autoencoder",
        **kwargs
    ):
        super().__init__(name=name, **kwargs)

        self.latent_dim = latent_dim
        self.encoder = create_cifar10_encoder(self.latent_dim)
        self.decoder = create_cifar10_decoder(self.latent_dim)
        self.beta = beta
        self.total_loss_tracker = keras.metrics.Mean(name="total_loss")
        self.reconstruction_loss_tracker = keras.metrics.Mean(name="reconstruction_loss")
        self.kl_loss_tracker = keras.metrics.Mean(name="kl_loss")

    @property
    def metrics(self):
        """Metrics of VAE."""
        
        return [
            self.total_loss_tracker,
            self.reconstruction_loss_tracker,
            self.kl_loss_tracker
        ]

    def train_step(self, data):
        """Make step when training."""
        
        x = data[0] if isinstance(data, tuple) else data
        with tf.GradientTape() as tape:
            # Forward pass
            mu, log_var, z = self.encoder(x)
            reconstruction = self.decoder(z)
            
            # Reconstruction loss (MSE)
            reconstruction_loss = tf.reduce_mean(
                tf.reduce_sum(
                    tf.square(x - reconstruction),
                    axis=(1, 2, 3)
                )
            )
            
            # KL divergence
            kl_loss = -0.5 * tf.reduce_sum(1 + log_var - tf.square(mu) - tf.exp(log_var), axis=1)
            kl_loss = tf.reduce_mean(kl_loss)
            
            # Total loss
            total_loss = reconstruction_loss + self.beta * kl_loss

        # Backpropagation
        grads = tape.gradient(total_loss, self.trainable_weights)
        self.optimizer.apply_gradients(zip(grads, self.trainable_weights))
        
        # Update metrics
        self.total_loss_tracker.update_state(total_loss)
        self.reconstruction_loss_tracker.update_state(reconstruction_loss)
        self.kl_loss_tracker.update_state(kl_loss)
        
        return {m.name: m.result() for m in self.metrics}

    def test_step(self, data):
        """Make step when validating."""
        
        x = data[0] if isinstance(data, tuple) else data

        mu, log_var, z = self.encoder(x)
        reconstruction = self.decoder(z)
        
        # Reconstruction loss (MSE)
        reconstruction_loss = tf.reduce_mean(
            tf.reduce_sum(
                tf.square(x - reconstruction),
                axis=(1, 2, 3)
            )
        )
        
        # KL divergence
        kl_loss = -0.5 * tf.reduce_sum(1 + log_var - tf.square(mu) - tf.exp(log_var), axis=1)
        kl_loss = tf.reduce_mean(kl_loss)
        
        # Total loss
        total_loss = reconstruction_loss + self.beta * kl_loss

        # Update metrics
        self.total_loss_tracker.update_state(total_loss)
        self.reconstruction_loss_tracker.update_state(reconstruction_loss)
        self.kl_loss_tracker.update_state(kl_loss)
        
        return {m.name: m.result() for m in self.metrics}

    def call(self, inputs):
        _, _, z = self.encoder(inputs)
        return self.decoder(z)
    
    
if __name__ == "__main__":
    latent_dim = 256
    batch_size = 128
    epochs = 30
    
    vae = VAE(latent_dim=latent_dim)

    vae.compile(optimizer=keras.optimizers.Adam(learning_rate=3e-4))
    vae.summary()

    (X_train, y_train), (X_test, y_test) = keras.datasets.cifar10.load_data()
    X_train = X_train.astype("float32") / 255.0
    X_test = X_test.astype("float32") / 255.0

    vae.fit(x=X_train,
            y=X_train,
            epochs=epochs,
            batch_size=batch_size,
            validation_split=0.1,
            verbose=1)

    visualize_reconstructions(vae, X_test, num_images=10)
    vae.save("vae.keras")

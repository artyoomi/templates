import keras
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE

from autoencoder import *

def compute_class_mean_reconstruction(X_data, y_data):
    """Compute and return mean mu and log_var for each class."""

    # Predict distribution
    prediction = vae.encoder.predict(X_data)

    avg_mu = np.zeros((10, 256), dtype=np.double)
    avg_log_var = np.zeros((10, 256), dtype=np.double)
    
    for i in range(len(prediction[0])):
        mu = prediction[0][i]
        log_var = prediction[1][i]
        tag = y_data[i][0]
        avg_mu[tag] += mu
        avg_log_var[tag] += log_var

    # Find mean in accumulated vectors
    for i in range(10):
        avg_mu[i] /= 6000
        avg_log_var[i] /= 6000

    return avg_mu, avg_log_var

def generate_samples_from_mean(decoder, avg_mu, avg_log_var):
    """Generate new images from mean for each class."""
    
    # Sample new images
    z_samples = np.zeros((50000, 256))
    for class_tag in range(10):
        print(f"Sampling {class_tag} class...")
        mu = avg_mu[class_tag]
        log_var = avg_log_var[class_tag]
        for i in range(5000):
            epsilon = tf.random.normal(shape=tf.shape(mu), dtype=np.double)
            z_samples[class_tag * 5000 + i] = mu + tf.exp(log_var * 0.5) * epsilon

    # Decode these averages
    x_samples = decoder.predict(z_samples)
    y_samples = np.repeat(np.arange(10), 5000).reshape(-1, 1)
    
    return x_samples, y_samples

def plot_latent_vectors(encoder, X_data, y_data):
    _, _, latent_vectors = np.array(encoder.predict(X_data))
    
    pca = PCA(n_components=50)
    pca_result = pca.fit_transform(latent_vectors)

    tsne = TSNE(n_components=3, verbose=1)
    tsne_result = tsne.fit_transform(pca_result)

    fig = plt.figure(figsize=(12, 10))
    ax = fig.add_subplot(111, projection="3d")
    scatter = ax.scatter(
        tsne_result[:, 0], 
        tsne_result[:, 1], 
        tsne_result[:, 2], 
        c=y_data, 
        cmap='tab10',
        alpha=0.6,
        s=10
    )

    # Legend with class names
    class_names = ['airplane', 'automobile', 'bird', 'cat', 'deer',
                   'dog', 'frog', 'horse', 'ship', 'truck']
    fig.legend(handles=scatter.legend_elements()[0], 
               labels=class_names,
               title="Classes")

    ax.set_xlabel('t-SNE Component 1')
    ax.set_ylabel('t-SNE Component 2')
    ax.set_zlabel('t-SNE Component 3')
    ax.set_title('Latent Space Visualization (t-SNE)')
    ax.view_init(elev=20, azim=45)
    plt.show()

if __name__ == "__main__":
    # Load model
    vae = keras.models.load_model("vae_90.keras")

    # Load dataset
    (X_train, Y_train), (X_test, Y_test) = keras.datasets.cifar10.load_data()
    X_train = X_train.astype("float32") / 255.0
    X_test = X_test.astype("float32") / 255.0

    # mu_mean, log_var_mean = compute_class_mean_reconstruction(X_train,
    #                                                           Y_train)
    # x_samples, y_samples = generate_samples_from_mean(vae.decoder,
    #                                                   mu_mean,
    #                                                   log_var_mean)
    # print(x_samples.shape)
    # print(y_samples.shape)

    # Decode these averages
    # result = vae.decoder.predict(x_samples)

    # x_samples = np.load("augmentation_samples_x.npy")
    # y_samples = np.load("augmentation_samples_y.npy")
    x_samples, y_samples = X_train, Y_train
    print(x_samples.shape)
    print(y_samples.shape)
    
    indices = np.random.choice(len(x_samples), 5000, replace=False)
    plot_latent_vectors(vae.encoder, x_samples[indices], y_samples[indices])

    # Save results
    # np.save("augmentation_samples_x.npy", x_samples)
    # np.save("augmentation_samples_y.npy", y_samples)

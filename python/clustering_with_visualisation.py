# LB1 from ETU AI development course

# 1) Задать N правил для генерации пятимерного облака из 50 точек (N выбрать произвольно в интервале [3;6])
# 2) Сгенерировать N облаков точек по каждому правилу 
# 3) С помощью PCA снизить размерность для каждого облака точек (независимо для каждого генерируюзщего правила) до 2 характеристик.
#    Должно получиться N*50 двумерных векторов, которые уже можно визуализировать.
# 4) Выполнить кластеризацию получившихся двумерных точек. Проверить, будет ли разделение на N кластеров - оптимально

import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score

N = 5

num_clusters = N
num_points_per_cluster = 50

# generate rules
rules = []
for i in range(N):
    center = np.random.uniform(-50.0, 50.0, N)
    spread = np.random.uniform(1.5, 3.5)
    rules.append((center, spread))

# generate point clouds
clouds = []
for center, spread in rules:
    cloud = np.random.normal(loc=center, scale=spread, size=(num_points_per_cluster, N))
    clouds.append(cloud)

# reduce dimension
pca = PCA(n_components=2)
clouds_2d = [pca.fit_transform(cloud) for cloud in clouds]
all_clouds_2d = np.vstack(clouds_2d)

fig = plt.figure()

inertia_values = []
silhouette_values = []

# try various number of clusters
K_range = range(2, 3*N)
for k in K_range:
    tmp_kmeans = KMeans(n_clusters=k)
    tmp_labels = tmp_kmeans.fit_predict(all_clouds_2d)

    inertia_values.append(tmp_kmeans.inertia_)
    silhouette_values.append(silhouette_score(all_clouds_2d, tmp_labels))

ax1 = fig.add_subplot(2, 2, 1)
ax2 = fig.add_subplot(2, 2, 2)
ax3 = fig.add_subplot(2, 1, 2)

ax1.set_title("Elbow Method")
ax1.plot(K_range, inertia_values, color="green", marker="o", linestyle="solid")

ax2.set_title("Silhouette Score")
ax2.plot(K_range, silhouette_values, color="black", marker="o", linestyle="solid")

# set kmeans to show on graph
kmeans = KMeans(n_clusters=N)
labels = kmeans.fit_predict(all_clouds_2d)

for i in range(N):
    ax3.scatter(all_clouds_2d[labels == i, 0], all_clouds_2d[labels == i, 1], label=f"Cluster {i + 1}")
ax3.set_title(f'PCA-Reduced 2D Data with KMeans Clustering (N={N})')
ax3.set_xlabel('PCA Component 1')
ax3.set_ylabel('PCA Component 2')
ax3.legend()
ax3.grid(True)

plt.show()

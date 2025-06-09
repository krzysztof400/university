from torchvision.datasets import EMNIST
from torchvision import transforms
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import gc

def initialize_centroids(X, k):
    n_samples, _ = X.shape
    centroids = []
    # Wybierz pierwszy centroid losowo
    first_centroid = X[np.random.randint(n_samples)]
    centroids.append(first_centroid)
    for _ in range(1, k):
        print(f"Wybieranie centroidu {_ + 1} z {k}...")
        # Oblicz minimalny kwadrat odległości od istniejących centroidów
        dist_sq = np.min([np.sum((X - c)**2, axis=1) for c in centroids], axis=0)
        probabilities = dist_sq / dist_sq.sum()
        cumulative_probs = np.cumsum(probabilities)
        r = np.random.rand()

        for j, p in enumerate(cumulative_probs):
            if r < p:
                centroids.append(X[j])
                break

    return np.array(centroids)

def kmeans(X, k, max_iter=10, tol=1e-4):
    centroids = initialize_centroids(X, k)
    print(f"Zainicjowano centroidy: {centroids}")
    for _ in range(max_iter):
        print(f"Iteracja {_ + 1} z {max_iter}...")
        # Przypisanie punktów do najbliższego centroidu
        distances = np.linalg.norm(X[:, np.newaxis] - centroids, axis=2)  # shape (n_samples, k)
        labels = np.argmin(distances, axis=1)

        # Oblicz nowe centroidy
        new_centroids = np.array([
            X[labels == i].mean(axis=0) if np.any(labels == i) else centroids[i]
            for i in range(k)
        ])

        # Sprawdź czy centroidy się zmieniły mniej niż tolerancja
        if np.all(np.linalg.norm(new_centroids - centroids, axis=1) < tol):
            break

        centroids = new_centroids

    # Oblicz inercję (suma kwadratów odległości do centroidów)
    inertia = sum(np.sum((X[labels == i] - centroids[i])**2) for i in range(k))

    return centroids, labels, inertia

def main():
    # Wczytaj dane EMNIST w wariancie "mnist"
    transform = transforms.ToTensor()

    dataset = EMNIST(root='./data', split='mnist', train=True, download=True, transform=transform)

    # Weź obrazy jako NumPy
    images = np.array([np.array(np.fliplr(np.rot90(img.squeeze(), k=-1))) for img, _ in dataset])
    labels = np.array([label for _, label in dataset])
    X = images.reshape(len(images), -1)

    # Zapisz dane w formacie, który może być łatwo odczytany przez program w C++
    with open("emnist_data.txt", "w") as f:
        for x, label in zip(X, labels):
            f.write(f"{','.join(map(str, x))}|{label}\n")

if __name__ == "__main__":
    main()
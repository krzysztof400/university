# Importowanie bibliotek
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, classification_report
from sklearn.model_selection import train_test_split
from tensorflow.keras.datasets import mnist  # MNIST z Keras
import numpy as np

# Wczytanie zbioru danych MNIST
(X_train_full, y_train_full), (X_test, y_test) = mnist.load_data()

# Przekształcenie danych (obrazy 28x28 pikseli na wektory 784-elementowe)
X_train_full = X_train_full.reshape((X_train_full.shape[0], -1))  # 60 000 próbek
X_test = X_test.reshape((X_test.shape[0], -1))  # 10 000 próbek

# Normalizacja wartości pikseli (0-255 -> 0-1), opcjonalne dla Random Forest, ale często stosowane
X_train_full = X_train_full / 255.0
X_test = X_test / 255.0

# Podział zbioru treningowego na treningowy i walidacyjny (dla porównania z testowym)
X_train, X_val, y_train, y_val = train_test_split(X_train_full, y_train_full, test_size=0.2, random_state=42)

# Stworzenie i trening klasyfikatora Random Forest
rf_classifier = RandomForestClassifier(n_estimators=100, random_state=42)
rf_classifier.fit(X_train, y_train)

# Predykcja na zbiorze testowym
y_pred = rf_classifier.predict(X_test)

# Obliczenie metryk
accuracy = accuracy_score(y_test, y_pred)
precision = precision_score(y_test, y_pred, average='weighted')
recall = recall_score(y_test, y_pred, average='weighted')

# Wyświetlenie wyników
print(f"Współczynnik prawidłowej rozpoznawalności (accuracy): {accuracy:.4f}")
print(f"Precyzja (precision): {precision:.4f}")
print(f"Czułość (recall): {recall:.4f}")
print("\nPełny raport klasyfikacji:")
print(classification_report(y_test, y_pred))
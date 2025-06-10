# filepath: /two_layer_nn_project/two_layer_nn_project/src/main.py

import numpy as np
from model import TwoLayerNN
from utils import l1_normalize

def main():
    # Initialize the neural network
    model = TwoLayerNN()

    # Input: two non-zero real numbers
    input_values = input("Enter two non-zero real numbers separated by a space: ")
    x1, x2 = map(float, input_values.split())

    # Normalize the input data
    normalized_input = l1_normalize(np.array([x1, x2]))

    print(f"Normalized input: {normalized_input}")

    # Get the prediction from the model
    prediction = model.predict(normalized_input)

    # Output the result
    print(f"The output is: {prediction}")

if __name__ == "__main__":
    main()
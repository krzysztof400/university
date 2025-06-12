# Two-Layer Neural Network Project

This project implements a two-layer neural network that takes two non-zero real numbers as input and determines if they have the same sign. The network outputs `1` if the inputs have the same sign and `0` otherwise. The project utilizes L1 normalization for input data and employs the ReLU activation function in its architecture.

## Project Structure

- **src/**
  - **main.py**: Entry point for the application. Initializes the neural network, processes input data, and outputs predictions.
  - **model.py**: Contains the implementation of the two-layer neural network, including architecture, weights, biases, and methods for forward propagation and activation.
  - **utils.py**: Provides utility functions, including L1 normalization and additional helper functions for data processing and model evaluation.
  - **tests/**
    - **test_model.py**: Contains unit tests for the neural network model, ensuring correctness of outputs and functionality of L1 normalization.

- **requirements.txt**: Lists the dependencies required for the project, such as NumPy and any other necessary libraries.

## Installation

To set up the project, clone the repository and install the required dependencies:

```bash
pip install -r requirements.txt
```

## Usage

To run the neural network, execute the following command:

```bash
python src/main.py
```

You will be prompted to enter two non-zero real numbers. The model will then output `1` if the numbers have the same sign and `0` otherwise.

## Testing

To run the unit tests for the model, navigate to the `src/tests` directory and execute:

```bash
python -m unittest test_model.py
```

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any suggestions or improvements.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
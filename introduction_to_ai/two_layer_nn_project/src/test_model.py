import unittest
import numpy as np
from model import TwoLayerNN
from utils import l1_normalize

class TestTwoLayerNN(unittest.TestCase):
    def setUp(self):
        self.model = TwoLayerNN()
    
    def test_same_sign(self):
        inputs = np.array([1.0, 2.0])
        normalized_inputs = l1_normalize(inputs)
        output = self.model.predict(normalized_inputs)
        self.assertEqual(output, 1)

        inputs = np.array([-1.0, -2.0])
        normalized_inputs = l1_normalize(inputs)
        output = self.model.predict(normalized_inputs)
        self.assertEqual(output, 1)

    def test_different_sign(self):
        inputs = np.array([1.0, -2.0])
        normalized_inputs = l1_normalize(inputs)
        output = self.model.predict(normalized_inputs)
        self.assertEqual(output, 0)

        inputs = np.array([-1.0, 2.0])
        normalized_inputs = l1_normalize(inputs)
        output = self.model.predict(normalized_inputs)
        self.assertEqual(output, 0)

if __name__ == '__main__':
    unittest.main()
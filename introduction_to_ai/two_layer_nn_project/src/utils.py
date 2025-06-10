def l1_normalize(inputs):
    norm = sum(abs(x) for x in inputs)
    if norm == 0:
        raise ValueError("L1 norm is zero, cannot normalize.")
    return [x / norm for x in inputs]

def preprocess_input(input1, input2):
    if input1 == 0 or input2 == 0:
        raise ValueError("Inputs must be non-zero real numbers.")
    return l1_normalize([input1, input2])
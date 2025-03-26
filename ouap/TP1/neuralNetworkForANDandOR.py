import numpy as np
import matplotlib.pyplot as plt

def sigmoid(x):
  # derivative of sigmoid: sigmoid'(x) = x'*(1−sigmoid(x))
  return 1.0 / (1.0 + np.exp(- x))

def sigmoid_derivative(x):
    return x * (1 - x)

# X = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])

# Y = np.array([[0], [0], [0], [1]])

# epochs = 10000
# lr = 0.1
# W = np.random.rand(2, 1)
# b = np.random.rand(1)

def relu(x):
  # derivative of relu(x) is the slope k = 1 or 0 if the value is 0
  return x * (x > 0)

def relu_derivative(x):
    return 1 * (x > 0)



###
N=100000 # number of iteration
alpha = 0.0001 # learning rate
def fit_with_one_nureon_in_layer_with_activation_neural_net(input, label, N=10000, alpha = 0.0001):
    w = np.random.normal(size=(2,), loc=0.1, scale=0.1) # initial values of weights
    b = np.random.normal(size=(1,), loc=0.1, scale=0.1)
    output = np.zeros(len(input)) # initial value of output
    error = np.zeros(len(input)) # error between output and label
    loss = []
    w_list = []
    for i in range(N):
        j = np.random.randint(len(input))

        Z = sigmoid(np.dot(input, w) + b)

        error = np.mean((label - Z)**2)

        derror_dZ = 2.0 * (label[j] - Z[j])
        dZ_dw = input[j] * sigmoid_derivative(Z[j])

        w = w + alpha * derror_dZ * dZ_dw
        b = b + alpha * derror_dZ


        if i % 1000 == 0:
            print("iteration: ", i, "loss: ", error)
            w_list.append(w.copy())
            loss.append(error)

    return Z, loss, w_list

def plot_simulation(output, label, title=""):
    plt.plot(output)
    plt.plot(label)
    plt.legend(('output','label'),loc='upper left')
    plt.title(title)
    plt.xlabel('number of samples')
    plt.ylabel('output value')
    plt.ylim(-0.1, 1.2)

def plot_loss(loss, title=""):
    plt.plot(loss)
    plt.legend(('neural net'),loc='upper left')
    plt.title(title)
    plt.xlabel('iterations')
    plt.ylabel('loss')
    plt.ylim(-0.1, 1.2)


dataset = {}
X = np.random.randint(0, 2, (200, 2))
dataset.update({"AND": {"X":X, "y":np.array([x[0] and x[1] for x in X], dtype=int)}})
dataset.update({"OR": {"X":X, "y":np.array([x[0] or x[1] for x in X], dtype=int)}})
dataset.update({"XOR": {"X":X, "y":np.array([(x[0] and not x[1]) or (not x[0] and x[1]) for x in X], dtype=int)}})

# output, loss, w_list = fit_with_one_nureon_in_layer_with_activation_neural_net(dataset["AND"]["X"], dataset["AND"]["y"], N=100000, alpha = 0.01)
# print(output, loss, w_list)

i = 1
fig, ax = plt.subplots(len(dataset), 3, figsize=(16, 12), sharex="col")

for name, ds in dataset.items():
    input = dataset[name]["X"]
    label = dataset[name]["y"]
    output, loss, w_list = fit_with_one_nureon_in_layer_with_activation_neural_net(input, label, N=100000, alpha = 0.01)
    plt.subplot(len(dataset), 3, i)
    i+=1
    plot_simulation(output, label, title="simulation of " + name)
    plt.subplot(len(dataset), 3, i)
    i+=1
    plot_loss(loss, title="loss of " + name)
    plt.subplot(len(dataset), 3, i)

    i+=1
    w_list2 = np.array(w_list)
    plt.plot(w_list2[:, 0:6])
    plt.plot(w_list2[:, 6:9], linestyle='dashed')
    plt.plot(w_list2[:, 9:12], linestyle='dashdot')
    plt.plot(w_list2[:, 12:13], linestyle='dotted')
    plt.title("weights of " + name)

    plt.legend(('w1_11', 'w1_12', 'w1_21', 'w1_22', 'w1_31', 'w1_32', 'b1_1', 'b1_2', 'b1_3',
                'w2_11', 'w2_12', 'w2_13', 'b2_1'),loc='upper right')

    plt.savefig(f"./{name}_plot.png")

    


fig.tight_layout(pad=1.0)

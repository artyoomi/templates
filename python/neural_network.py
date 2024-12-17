# lb2 from ETU AI development course

import random
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import numpy as np

VALUES_FOR_TRAINING   = 10000
VALUES_FOR_VALIDATION = 100
EPOCHS_COUNT          = 20
WEIGHT_CHANGE_LAMBDA  = 0.001
CONFIDENCE_INTERVAL   = 0.95

def passing_through_network(x, y, weights):
    def linear_activator(x):  return x
    def sigmoid_activator(x): return 1 / (1 + np.exp(-x))

    first_result  = linear_activator(x * weights[0])    
    second_result = linear_activator(y * weights[1])

    return sigmoid_activator(weights[2] * first_result + weights[3] * second_result)

"""
Input: number of points to generate and function to get answers
Output: tuple of three objects: x values, y values, and correct answers
"""
def generate_points(n_values, validation_f = lambda x, y: -x < y):
    x_values    = []
    y_values    = []
    correct_ans = []
    for _ in range(n_values):
        x_values.append(random.randint(-100, 100))
        y_values.append(random.randint(-100, 100))
        correct_ans.append(1 if validation_f(x_values[-1], y_values[-1]) else 0)

    return (x_values, y_values, correct_ans)

def gradient_const_part(calculated_exp, ans):
    return 2 * (1 / (1 + calculated_exp) - ans) * (calculated_exp / (1 + calculated_exp)**2)

if (__name__ == "__main__"):
    # generate points for training
    x_for_train, y_for_train, correct_predict = generate_points(VALUES_FOR_TRAINING)

    # generate initial weights
    weights = [random.uniform(-1, 1) for _ in range(4)]
    print("Initial weights:", *map(lambda num: f"{num:.5f}", weights))

    # neural network training
    for _ in range(EPOCHS_COUNT):
        for i in range(VALUES_FOR_TRAINING):
            curr_x, curr_y = x_for_train[i], y_for_train[i]

            w0, w1, w2, w3 = weights[0], weights[1], weights[2], weights[3]

            exp_degree = -w0 * w2 * curr_x - w1 * w3 * curr_y
            calculated_exp = np.exp(exp_degree)
            weight_change_const_part = WEIGHT_CHANGE_LAMBDA * gradient_const_part(calculated_exp, correct_predict[i])
            weights[0] -= weight_change_const_part * w2 * curr_x
            weights[1] -= weight_change_const_part * w3 * curr_y
            weights[2] -= weight_change_const_part * w0 * curr_x
            weights[3] -= weight_change_const_part * w1 * curr_y
            

    print("Weights of the trained model:", *map(lambda num: f"{num:.5f}", weights))

    # generate validation points
    x_for_valid, y_for_valid, correct_ans = generate_points(VALUES_FOR_VALIDATION)

    # checking the training
    model_ans = []
    correct_count = 0
    for i in range(VALUES_FOR_VALIDATION):
        model_ans.append(np.round(passing_through_network(x_for_valid[i], y_for_valid[i], weights), 4) >= CONFIDENCE_INTERVAL)
        if correct_ans[i] == model_ans[i]:
            correct_count += 1
        else:
            # to mark wrong prediction
            model_ans[i] = 2
    print(f"Percentage of values defined correctly: {correct_count / VALUES_FOR_VALIDATION:.2%}")

    # plot results
    fig = plt.figure()

    # change colors for different points
    for i in range(len(model_ans)):
        match (model_ans[i]):
            # above y = -x
            case 1:
                model_ans[i] = "green"
            # below y = -x
            case 0:
                model_ans[i] = "yellow"
            # wrong prediction
            case 2:
                model_ans[i] = "red"
            

    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2)

    ax1.grid(ls=':')
    ax1.set_title("Training values")
    ax1.scatter(x_for_train, y_for_train, c=correct_predict, s=3)

    ax2.grid(ls=':')
    ax2.axline((0, 0), slope=-1, color='black', linestyle='--', alpha=0.5, linewidth=1)
    ax2.set_title("The result of the training check")
    ax2.scatter(x_for_valid, y_for_valid, c=model_ans)
    
    legend_elements = [
        Line2D([0], [0], marker='o', color='w', label='Above y = -x', 
                 markerfacecolor='green', markersize=10),
        Line2D([0], [0], marker='o', color='w', label='Below y = -x', 
                 markerfacecolor='yellow', markersize=10),
        Line2D([0], [0], marker='o', color='w', label='Wrong predict', 
                 markerfacecolor='red', markersize=10),
    ]
    plt.legend(handles=legend_elements, loc='upper right')
        
    plt.show()
    

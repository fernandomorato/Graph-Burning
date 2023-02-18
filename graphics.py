# Arquivo disponibilizado por Felipe

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# plt.rcParams.update(plt.rcParamsDefault)

plt.rcParams.update({'figure.max_open_warning': 0})

#Set fonts
plt.rc('text', usetex=True)
plt.rc('font', family='serif')

# Using readlines()
file = open('alpha_results.csv', 'r')
lines = file.readlines()
  
n_alphas = 10
n_plots_per_instance = 4

# Strips the newline character
for line in lines:
    splitted_line = line.split(",")
    instance = str(splitted_line[0])

    x_alphas = ["0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1.0"]
    y_iters = []
    y_freqs = []
    y_bs = []
    y_avgs = []

    column = 1
    for i in range(n_alphas):
        y_iters.append(int(splitted_line[column]))
        y_freqs.append(int(splitted_line[column+1]))
        y_bs.append(float(splitted_line[column+2]))
        y_avgs.append(float(splitted_line[column+3]))
        column += n_plots_per_instance

    
    fig = plt.figure(figsize = (10, 5))
    plt.grid(axis="y")
    plt.xlabel(r"$\alpha$", fontsize=18)
    plt.ylabel("Iterations", fontsize=18)
    plt.xticks(fontsize=18)
    plt.yticks(fontsize=18)
    plt.bar(x_alphas, y_iters, color ='blue')
    plt.savefig("graphics/" + instance + "_iter.svg", bbox_inches='tight')
    plt.clf()

    fig = plt.figure(figsize = (10, 5))
    plt.grid(axis="y")
    plt.xlabel(r"$\alpha$", fontsize=18)
    plt.ylabel("Iterations", fontsize=18)
    plt.ylabel("Best solution frequency", fontsize=18)
    plt.xticks(fontsize=18)
    plt.yticks(fontsize=18)
    plt.bar(x_alphas, y_freqs, color ='pink')
    plt.savefig("graphics/" + instance + "_freq.svg", bbox_inches='tight')
    plt.clf()

    fig = plt.figure(figsize = (10, 5))
    plt.grid(axis="y")
    plt.xlabel(r"$\alpha$", fontsize=18)
    barWidth = 0.4
    br1 = np.arange(n_alphas)
    br2 = [x + barWidth for x in br1]
    plt.bar(br1, y_bs, color ='#0082FF', width = barWidth, edgecolor ='grey', label ='Best solution value')
    plt.bar(br2, y_avgs, color ='#FF7D00', width = barWidth, edgecolor ='grey', label ='Average solution value')
    plt.xticks([r + barWidth/2 for r in range(n_alphas)], x_alphas, fontsize=18)
    plt.yticks(fontsize=18)
    plt.legend(fontsize=18)
    plt.ylim(min(min(y_bs), min(y_avgs))*0.9, max(max(y_bs), max(y_avgs))*1.01)
    plt.savefig("graphics/" + instance + "_sol.svg", bbox_inches='tight')
    plt.clf()
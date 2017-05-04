import numpy as np
import matplotlib.pyplot as plt

t = 1.959964
t_y = 3.983

x = []
y = []
lines = []

file_in = open('10_20_err.var', 'r')

for line in file_in:
    lines.append(line)

for line in lines:
    y.append(float(line))

for t in range(len(y)):
    x.append(0.5*t)

fig = plt.figure()
ax = fig.gca()
ax.set_xticks(np.arange(0, max(x), 10))

plt.plot(x, y, 'k-')
plt.grid()

plt.show()

import numpy as np
import matplotlib.pyplot as plt
import pylab as pl

t = 1.959964
t_y = 3.983

data = np.loadtxt('test.var')

pl.plot(np.arange(0, len(data), 1), data)
pl.grid(True)
pl.show()

# for line in file_in:
#     lines.append(line)
#
# for line in lines:
#     y.append(float(line))
#
# for t in range(len(y)):
#     x.append(0.5*t)
#
# fig = plt.figure()
# ax = fig.gca()
# ax.set_xticks(np.arange(0, max(x), 10))
#
# plt.plot(x, y, 'k-')
# plt.grid()
#
# plt.show()

import numpy as np
import pylab as pl

data_in = np.loadtxt("text.in")
data_curr = np.loadtxt("text.curr")
data_out = np.loadtxt("text.out")

pl.plot(data_in[:,0], data_in[:,1], 'r-')
pl.plot(data_out[:,0], data_out[:,1], 'g--')
pl.xlabel('x')
pl.ylabel('y')
pl.xlim(0.0)

pl.show()

pl.plot(data_curr[:,0], data_curr[:,1], 'b--')

pl.show()

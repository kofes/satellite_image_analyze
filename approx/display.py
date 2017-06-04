import numpy as np
import matplotlib.pyplot as plt
import pylab as pl

def approx (x, lsq):
    i = lsq.size-1
    if i == 0:
        return [lsq]*len(x)
    res = 0
    for coef in lsq:
        res += coef*(x**i)
        i-=1
    return res

var = np.loadtxt('test.var')
lsq = np.loadtxt('test.lsq')

x = np.arange(0, len(var), 1)

pl.plot(x, var, color='r')
pl.plot(x, approx(x, lsq), color='g')
pl.grid(True)
pl.show()

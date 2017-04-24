import numpy as np
import matplotlib.pyplot as plt

x = []
y = []
dh = 0
E, D = 0, 0

def phi (u):
    return (1/np.sqrt(2*np.pi)*np.exp(-u*u/2))

def F (X):
    return (sum(x)*dh/D)*phi((X-E)/D)

with open('test.var') as fp0:
    minV, maxV, dh = [int(x) for x in next(fp0).split()]
    print(minV, maxV, dh)
    for i in range(minV, maxV):
        x.append(i*dh)
        y.append(float(fp0.readline()))
    plt.plot(x, y, 'k-')

with open('tmp.var') as fp1:
    E, D = [float(x) for x in next(fp1).split()]
    print(E, D)
    plt.plot((E, E), (0, max(y)), 'b-')
    plt.plot((E+np.sqrt(D), E+np.sqrt(D)), (0, max(y)), 'r-')
    plt.plot((E-np.sqrt(D), E-np.sqrt(D)), (0, max(y)), 'r-')

_X_ = np.arange(0, maxV, 1);
_Y_ = F(_X_)

plt.plot(_X_, _Y_)
plt.show()

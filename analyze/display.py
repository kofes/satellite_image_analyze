import numpy as np
import matplotlib.pyplot as plt

t = 1.959964
t_y = 3.983

x = []
y = []
maxV, minV, dh = 0, 0, 0
E1, D1 = 0, 0
E2, D2 = 0, 0

def phi (u):
    return (1/np.sqrt(2*np.pi))*np.exp(-(u**2 / 2))

def F (X, E, D):
    return (dh*sum(y)/np.sqrt(D))*phi((X-E)/np.sqrt(D))

with open('test.var') as fp0:
    minV, maxV, dh = [int(x) for x in next(fp0).split()]
    print(minV, maxV, dh)
    for i in range(minV, maxV):
        x.append(i*dh)
        y.append(float(fp0.readline()))
    plt.plot(x, y, 'k-')

with open('tmp.var') as fp1:
    E1, D1 = [float(x) for x in next(fp1).split()]
    E1-=1
    E2, D2 = [float(x) for x in next(fp1).split()]
    E2-=1
    print(E1, D1)
    print(E2, D2)
    plt.plot((E1, E1), (0, max(y)), 'b--')
    plt.plot((E1+np.sqrt(D1), E1+np.sqrt(D1)), (0, max(y)), 'g--')
    plt.plot((E1-np.sqrt(D1), E1-np.sqrt(D1)), (0, max(y)), 'g--')
    plt.plot((E2, E2), (0, max(y)), 'r-.')
    plt.plot((E2+np.sqrt(D2), E2+np.sqrt(D2)), (0, max(y)), 'y-.')
    plt.plot((E2-np.sqrt(D2), E2-np.sqrt(D2)), (0, max(y)), 'y-.')
    D1 = (dh*sum(y) / (y[int(E1)-minV]))**2/(2*np.pi)
    D2 = (dh*sum(y) / (y[int(E2)-minV]))**2/(2*np.pi)
# plt.plot((E1+t*np.sqrt(D*((maxV-minV+1)/(maxV-minV))), E+t*np.sqrt(D*((maxV-minV+1)/(maxV-minV)))), (0, max(y)), 'r-')
# plt.plot((E1-t*np.sqrt(D*((maxV-minV+1)/(maxV-minV))), E-t*np.sqrt(D*((maxV-minV+1)/(maxV-minV)))), (0, max(y)), 'r-')

_X_ = np.arange(0, maxV, dh);
_Y_ = F(_X_, E1, D1) + F(_X_, E2, D2)

plt.plot(_X_, _Y_)
plt.show()

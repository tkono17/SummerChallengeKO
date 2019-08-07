import matplotlib.pyplot as plt
import numpy as np
import sys

args = sys.argv

#initial value mm
a = float(args[1]) #0.07
b = float(args[2]) #0.02
L = float(args[3]) #262.5
lam = 0.625e-3

p = np.pi
k = 2*p/lam
theta = np.linspace(-1, 1, 100000) #variable
sin = np.sin(theta)
alpha = k*a*sin/2
beta = k*b*sin/2

z = L*sin
I1 = 4*(np.sin(beta)*np.sin(beta)/(beta*beta))*np.cos(alpha)*np.cos(alpha)
I2 = 4*(np.sin(beta)*np.sin(beta)/(beta*beta))

xlim = 15

m_a = lam*L/(2*a)
m_b = lam*L/b

print ('interbal by alpha : {}'.format(m_a))
print ('interbal by beta  : {}'.format(m_b))

plt.figure(figsize=(7, 7))
#ax = fig.add_subplot(1, 1, 1)
plt.plot(z, I1)
plt.plot(z, I2)
plt.title('(a, b, L) = ({0}, {1}, {2})'.format(a, b, L), fontsize=18)
plt.legend(['alpha', 'beta'], fontsize=18)
plt.xlim(-xlim, xlim)
#plt.locator_params(axis='x', tight=True, nbins=xlim*2)
#plt.xticks(np.arange(-10, 10, 20))
plt.xlabel('z [mm]', fontsize=18)
plt.ylabel('Intensity', fontsize=18)
plt.tick_params(labelsize=18)

plt.savefig('2slitsred_{0}_{1}_{2}.png'.format(a, b, L))
plt.show()

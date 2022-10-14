from scipy.stats import norm
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

# read data from a text file. One number per line
arch = "time_diff.txt"
data = []
for item in open(arch,'r'):
    item = item.strip()
    data.append(float(item))

# best fit of data
(mu, sigma) = norm.fit(data,method="MLE")

# the histogram of the data
n, bins, patches = plt.hist(data, 20, normed=1, facecolor='green', alpha=0.75)

# add a 'best fit' line
y = mlab.normpdf( bins, mu, sigma)
l = plt.plot(bins, y, 'r--', linewidth=2)

#plot
plt.xlabel('$\Delta t$')
plt.ylabel('')
#plt.title('$std=%.3f$' %(sigma))
plt.grid(True)
print('Std = %.3f' % sigma)
plt.savefig("time_diff.pdf")
plt.show()

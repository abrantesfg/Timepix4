import numpy as np
import math
from scipy.stats import norm
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import sys, os

arch = "correlation/t_scint"+sys.argv[1]+".txt" #t_scint1.txt"
arch2 = "correlation/t_mcp.txt"

dataOutput = open("correlation/mcp_scint"+sys.argv[1]+"_corr.txt","w+")

t_scint = []
t_mcp = []
diff = []

for item in open(arch,'r'):
    item = item.strip()
    t_scint.append(float(item))

print(len(t_scint))

for item in open(arch2,'r'):
    item = item.strip()
    t_mcp.append(float(item))

print(len(t_mcp))



j_min = 0
time_window = 100
for i in range(len(t_mcp)):
    for j in range(j_min, len(t_scint)):
        dt = t_scint[j] - t_mcp[i]
        if dt < - 100:
            j_min = j
            continue
        elif dt < 100:
            diff.append(dt)
            dataOutput.write(f"{dt}\n")
        else:
            break


# Plotting difference t_scint - t_mcp
fig, ax = plt.subplots()

# best fit of data
(mu, sigma) = norm.fit(diff,method="MLE")

# the histogram of the data
n, bins, patches = plt.hist(diff, 50, density=True, facecolor='green', alpha=0.75)

# add a 'best fit' line
y = norm.pdf( bins, mu, sigma)
#l = plt.plot(bins, y, 'r--', linewidth=2)

plt.xlabel('Correlation')
plt.ylabel('')
plt.title('Scint'+sys.argv[1]+' vs MCP')
plt.grid(True)
print('mu = %.3f' % mu)
print('sigma = %.3f' % sigma)

#textstr = '\n'.join((
#    r'$\mu=%.2f$' % (mu, ),
#    r'$\sigma=%.2f$' % (sigma, )))

# these are matplotlib.patch.Patch properties
#props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)


# place a text box in upper left in axes coords
#ax.text(0.05, 0.95, textstr, transform=ax.transAxes, fontsize=14,
#        verticalalignment='top', bbox=props)
plt.savefig("correlation/corr_scint"+sys.argv[1]+"_mcp.pdf")
plt.show()

plt.hist(t_mcp, 200, facecolor='green', alpha=0.75)
plt.show()

dataOutput.close()
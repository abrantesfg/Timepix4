import numpy as np
import math
from scipy.stats import norm
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

arch = "signal_width/t1_times.txt"
arch2 = "signal_width/t2_times.txt"
dataOutput = open("signal_width/time_diff.txt","w+")

t1 = []
t2 = []
diff = []

for item in open(arch,'r'):
    item = item.strip()
    t1.append(float(item))



for item in open(arch2,'r'):
    item = item.strip()
    t2.append(float(item))

for i in range(len(t1)):
    diff = t1[i] - t2[i]
    dataOutput.write(str(diff)+ "\n")

t1.close()
t2.close()
dataOutput.close()


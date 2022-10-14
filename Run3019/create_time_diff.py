import numpy as np
import math

arch = "t1_times.txt"
arch2 = "t2_times.txt"
dataOutput = open("time_diff.txt","w+")

t1 = []
t2 = []

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


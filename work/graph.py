from matplotlib import pyplot as plt 
import numpy as np 

def read_txt(filename):
    txt = open(filename, 'r')
    txt = txt.readlines()
    txt.pop(0)
    values = []
    for line in txt:
        values.append(float(line))
    return np.array(values)


filename1 = "tblfile.txt"
filename = "xvector.txt"
x = read_txt(filename)
tbl = read_txt(filename1)
plt.plot(x[0:2500], 'r.', markersize=12)
plt.plot(tbl[0:2500], 'g-')
plt.title("Array generado a partir de tbl")
plt.legend(["Samples", "Tbl "], loc='upper right')
plt.autoscale(enable=True, axis='x', tight=True)
plt.show()
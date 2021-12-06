import numpy as np
f = open('position.txt', 'w')
for x in np.arange(9,12, 0.1):
    f.write(str(x))
    f.write('\n')
f.close()

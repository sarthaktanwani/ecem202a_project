import numpy as np

f = open ("next_position.txt", 'w')

# Start Values 
# x = -11.56
# y = -7.12
# z = 0.128750
# Z value remains constant throughout simulation

f.write('Start \n')
for x in np.arange(-11.56,6.16,0.5):
    f.write(str(x))
    f.write('\n')

f.write('First Checkpoint \n')
for y in np.arange(-7.12,6.88,0.5):
    f.write(str(y))
    f.write('\n')

f.write('Second Checkpoint \n')
for x in np.arange(6.16,-16.24,-0.5):
    f.write(str(x))
    f.write('\n')

f.write('Third Checkpoint \n')
for y in np.arange(6.88,1.72,-0.5):
    f.write(str(y))
    f.write('\n')

f.write('Fourth Checkpoint \n')
for x in np.arange(-16.24,-5.21,0.5):
    f.write(str(x))
    f.write('\n')
f.write('end')

f.close()
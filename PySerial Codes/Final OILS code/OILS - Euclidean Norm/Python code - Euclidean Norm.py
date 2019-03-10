import serial
import struct
import numpy as np
import math
port=serial.Serial('COM4',9600)
print("Port Accepted")

#arr = np.array([1,2,3,4,5,6,7,8,9,10,math.pi,math.sqrt(2),math.e])
arr = 10000*np.random.random_sample((13,2)) - 5000

print('Input 1','\t','Input 2','\t','Output','\t', 'Actual Value','\t','Error' )

for i in np.arange(13):
    
    elem1 = arr[i][0]
    elem1 = float(elem1)
    elem2 = arr[i][1]
    elem2 = float(elem2)
    
    #print(' ')
    #print (elem1)
    #print (elem2)
    #print((struct.pack('<f', elem1)))
    #print((struct.pack('<f', elem2)))
    port.write(b"\x61")
    port.write((struct.pack('<f', elem1)))
    port.write(b"\x61")
    port.write((struct.pack('<f', elem2)))
    #print("Variable sent")

    port.flushInput()
    a1 = port.read(4)
    #print(a1)
    out = struct.unpack('=f', a1)[0]
    act = math.sqrt(elem1**2 + elem2**2)
    if (out == 0):
        err = 0
    else:
        err = (out - act)/(act)
    print(elem1,'\t',elem2,'\t',out,'\t', act,'\t',err )

    

input()
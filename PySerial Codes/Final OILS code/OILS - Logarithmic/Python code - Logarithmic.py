import serial
import struct
import numpy as np
import math
port=serial.Serial('COM4',9600)
print("Port Accepted")

arr = np.array([1,2,3,4,5,6,7,8,9,10,math.pi,math.sqrt(2),math.e])

print('Input','\t','Output','\t', 'Actual Value','\t','Error' )

for i in np.arange(13):
    ##elem = np.float32(elem)
    elem = arr[i]
    elem = float(elem)
    #print (elem)
    #print((struct.pack('<f', elem)))
    port.write(b"\x61")
    port.write((struct.pack('<f', elem)))
    #print("Variable sent")

    port.flushInput()
    a1 = port.read(4)
    #print(a1)
    out = struct.unpack('=f', a1)[0]
    if (math.log(elem) == 0):
        err = 0
    else:
        err = (out - math.log(elem))/(math.log(elem))
    print(elem,'\t',out,'\t', math.log(elem),'\t',err )

    

input()
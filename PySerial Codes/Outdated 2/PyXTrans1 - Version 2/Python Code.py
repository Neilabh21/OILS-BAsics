import serial
import struct
port=serial.Serial('COM4',9600)
print("Port Accepted")


#input("Start sending?")

#w = port.read()
#print(chr(int.from_bytes(w,byteorder='little')))

def float_to_bytes(f):
    return (struct.pack('>f', float1))

#float1 = 1.13123123
float1 = input("Input a float to send: ")
float1 = float(float1)

#hex1 = float_to_hex(float1)

#send_data = bytes.fromhex(str(hex1)[2:])
print((struct.pack('>f', float1)))
port.write(b"\x61")
port.write((struct.pack('>f', float1)))
print("Variable sent")

#input("End sending?")
#port.write(End)
#print("End send")

port.flushInput()
a1 = port.read(4)
print(a1)
print(struct.unpack('>f', a1)[0])
print(struct.unpack('<f', a1)[0])

#print(chr(int.from_bytes(w,byteorder='little')))


#input()
exit()
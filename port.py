import serial
import numpy as np


ser = serial.Serial(port = "COM9", baudrate = 4800, bytesize = 8, timeout = 1, stopbits = serial.STOPBITS_ONE)
ser.close()
ser.open()
while True:
    if(ser.in_waiting > 0):
        data = ser.readline()
        current = int(data.decode())
        print(current+10)



# Importing Libraries
import serial
import time
arduino = serial.Serial(port='COM3', baudrate=57600, timeout=.1)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    data1 = arduino.readline()
    data2 = arduino.readline()
    data3 = arduino.readline()
    data4 = arduino.readline()
    data5 = arduino.readline()
    return data1 + data2 + data3 + data4 + data5
while True:
    message = input("Send a message: ") # Taking input from user
    value = write_read(message)
    arduino.write(bytes("S=", 'utf-8'))
    print(value) # printing the value
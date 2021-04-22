# Importing Libraries
import serial
import time
arduino = serial.Serial(port='/dev/ttyACM0', baudrate=57600, timeout=.1)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.1)
    data1 = arduino.readline()
    data2 = arduino.readline()
    data3 = arduino.readline()
    data4 = arduino.readline()
    data5 = arduino.readline()
    return data1 + data2 + data3 + data4 + data5
while True:
    num = input("Enter a number: ") # Taking input from user
    value = write_read(num)
    print(value) # printing the value
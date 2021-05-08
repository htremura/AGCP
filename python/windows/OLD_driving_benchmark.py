import serial
import time
import numpy as np

class data:
    rl    =   np.array([])
    r5l =   np.array([])
    r50l =   np.array([])
    r100l =   np.array([])

    fl    =   np.array([])
    f5l =   np.array([])
    f50l =   np.array([])
    f100l =   np.array([])

    sl    =   np.array([])

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
writefile = input("Which file should be written?")
count = int(input("How many runs should we try?"))
time.sleep(2)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    data1 = arduino.readline()
    return data1.decode("utf-8")
try:
    for x in range(count):
        F = write_read("F:")
        data.fl = np.append(data.fl, int(F))
        time.sleep(0.15)
        S = int(write_read("S:"))
        time.sleep(0.05)

        F5 = write_read("F:5")
        data.f5l = np.append(data.f5l, int(F5))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        F50 = write_read("F:50")
        data.f50l = np.append(data.f50l, int(F50))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        F100 = write_read("F:100")
        data.f100l = np.append(data.f100l, int(F100))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        R = write_read("R:")
        data.rl = np.append(data.rl, int(R))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        R5 = write_read("R:5")
        data.r5l = np.append(data.r5l, int(R5))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        R50 = write_read("R:50")
        data.r50l = np.append(data.r50l, int(R50))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        R100 = write_read("R:100")
        data.r100l = np.append(data.r100l, int(R100))
        time.sleep(0.15)
        S += int(write_read("S:"))
        data.sl = np.append(data.sl, S/8)
        time.sleep(0.05)

except KeyboardInterrupt:
    pass
finally:

    t = open("olddrive_" + writefile + "a.txt", "w+")
    t.write(\
    "TR avg: " + str(np.average(data.rl)*(1/(16000000))*10**6) + " us\nTR5 avg: " + str(np.average(data.r5l)*(1/(16000000))*10**6) + "us\nTR50 avg: " + str(np.average(data.r50l)*(1/(16000000))*10**6) + " us\nTR100 avg: " + str(np.average(data.r100l)*(1/(16000000))*10**6) + \
    "us\n\nF avg: " + str(np.average(data.fl)*(1/(16000000))*10**6) + "us\nF5 avg: " + str(np.average(data.f5l)*(1/(16000000))*10**6) + "us\nF50 avg: " + str(np.average(data.f50l)*(1/(16000000))*10**6) + " us\nTR100 avg: " + str(np.average(data.f100l)*(1/(16000000))*10**6) + \
    "us\n\nS avg: " + str(np.average(data.sl)*(1/(16000000))*10**6) + "us")

    fields   = np.matrix(['TR', 'TR5', 'TR50', 'TR100', 'F', 'F5', 'F50', 'F100', 'S'])
    complete = np.matrix([data.rl, data.r5l, data.r50l, data.r100l, data.fl, data.f5l, data.f50l, data.f100l, data.sl])
    with open("olddrive_" + writefile + "np.csv", "a") as analysis:
#        analysis.write(str(fields) + "\n")
        np.savetxt(analysis, np.hstack(fields), delimiter=",", fmt="%s")
        np.savetxt(analysis, np.transpose(complete), delimiter=",", fmt='%1.0f')

        

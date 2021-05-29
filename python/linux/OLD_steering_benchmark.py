import serial
import time
import numpy as np

class data:
    rl    =   np.array([])
    r5l =   np.array([])
    r50l =   np.array([])
    r100l =   np.array([])

    ll    =   np.array([])
    l5l =   np.array([])
    l50l =   np.array([])
    l100l =   np.array([])

    sl    =   np.array([])

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.1)
writefile = input("Which file should be written?")
count = int(input("How many runs should we try?"))
time.sleep(2)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    data1 = arduino.readline()
    return data1.decode("utf-8")
try:
    for x in range(count):
        TR = write_read("TR:")
        data.rl = np.append(data.rl, int(TR))
        time.sleep(0.15)
        S = int(write_read("S:"))
        time.sleep(0.05)

        TR5 = write_read("TR:5")
        data.r5l = np.append(data.r5l, int(TR5))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        TR50 = write_read("TR:50")
        data.r50l = np.append(data.r50l, int(TR50))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        TR100 = write_read("TR:100")
        data.r100l = np.append(data.r100l, int(TR100))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        TL = write_read("TL:")
        data.ll = np.append(data.ll, int(TL))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        TL5 = write_read("TL:5")
        data.l5l = np.append(data.l5l, int(TL5))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        TL50 = write_read("TL:50")
        data.l50l = np.append(data.l50l, int(TL50))
        time.sleep(0.15)
        S += int(write_read("S:"))
        time.sleep(0.05)

        TL100 = write_read("TL:100")
        data.l100l = np.append(data.l100l, int(TL100))
        time.sleep(0.15)
        S += int(write_read("S:"))
        data.sl = np.append(data.sl, S/8)
        time.sleep(0.05)
except KeyboardInterrupt:
    pass
finally:

    t = open("oldsteer_" + writefile + "a.txt", "w+")
    t.write(\
    "TR avg: " + str(np.average(data.rl)*(1/(16000000))*10**6) + " us\nTR5 avg: " + str(np.average(data.r5l)*(1/(16000000))*10**6) + "us\nTR50 avg: " + str(np.average(data.r50l)*(1/(16000000))*10**6) + " us\nTR100 avg: " + str(np.average(data.r100l)*(1/(16000000))*10**6) + \
    "us\n\nTL avg: " + str(np.average(data.ll)*(1/(16000000))*10**6) + "us\nTL5 avg: " + str(np.average(data.l5l)*(1/(16000000))*10**6) + "us\nTL50 avg: " + str(np.average(data.l50l)*(1/(16000000))*10**6) + " us\nTR100 avg: " + str(np.average(data.r100l)*(1/(16000000))*10**6) + \
    "us\n\nS avg: " + str(np.average(data.sl)*(1/(16000000))*10**6) + "us")

    fields   = np.matrix(['TR', 'TR5', 'TR50', 'TR100', 'TL', 'TL5', 'TL50', 'TL100', 'S'])
    complete = np.matrix([data.rl, data.r5l, data.r50l, data.r100l, data.ll, data.l5l, data.l50l, data.r100l, data.sl])
    with open("oldsteer_" + writefile + "np.csv", "a") as analysis:
#        analysis.write(str(fields) + "\n")
        np.savetxt(analysis, np.hstack(fields), delimiter=",", fmt="%s")
        np.savetxt(analysis, np.transpose(complete), delimiter=",", fmt='%1.0f')

        

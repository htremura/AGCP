import serial
import time
import numpy as np

class data:
    f_l    =   np.array([])
    f_5l =   np.array([])
    f_50l =   np.array([])
    f_100l =   np.array([])

    r_l    =   np.array([])
    r_5l =   np.array([])
    r_50l =   np.array([])
    r_100l =   np.array([])

    invl =   np.array([])

    sl    =   np.array([])

arduino = serial.Serial(port='COM3', baudrate=57600, timeout=.1)
writefile = input("Which file should be written?")
count = int(input("How many runs should we try?"))
time.sleep(2)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    data1 = arduino.readline()
    return data1.decode("utf-8")
try:
    for x in range(count):
        F_ = write_read("F_=")
        data.f_l = np.append(data.f_l, int(F_))
        time.sleep(0.15)
        S = int(write_read("S="))
        time.sleep(0.05)

        F_5 = write_read("F_5=")
        data.f_5l = np.append(data.f_5l, int(F_5))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        F_50 = write_read("F_50=")
        data.f_50l = np.append(data.f_50l, int(F_50))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        F_100 = write_read("F_100=")
        data.f_100l = np.append(data.f_100l, int(F_100))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        R_ = write_read("R_=")
        data.r_l = np.append(data.r_l, int(R_))
        time.sleep(0.15)
        S = int(write_read("S="))
        time.sleep(0.05)

        R_5 = write_read("R_5=")
        data.r_5l = np.append(data.r_5l, int(R_5))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        R_50 = write_read("R_50=")
        data.r_50l = np.append(data.r_50l, int(R_50))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        R_100 = write_read("R_100=")
        data.r_100l = np.append(data.r_100l, int(R_100))
        time.sleep(0.15)
        S += int(write_read("S="))
        data.sl = np.append(data.sl, S/8)
        time.sleep(0.05)

        INV = write_read("INV=")
        data.invl = np.append(data.invl, int(INV))
        time.sleep(0.05)

except KeyboardInterrupt:
    pass
finally:

    t = open("drive_" + writefile + "a.txt", "w+")
    t.write(\
    "R_ avg: " + str(np.average(data.r_l)*(1/(16000000))*10**6) + " us\nR_5 avg: " + str(np.average(data.r_5l)*(1/(16000000))*10**6) + "us\nR_50 avg: " + str(np.average(data.r_50l)*(1/(16000000))*10**6) + " us\nR_100 avg: " + str(np.average(data.r_100l)*(1/(16000000))*10**6) + \
    "us\n\nF_ avg: " + str(np.average(data.f_l)*(1/(16000000))*10**6) + "us\nF_5 avg: " + str(np.average(data.f_5l)*(1/(16000000))*10**6) + "us\nF_50 avg: " + str(np.average(data.f_50l)*(1/(16000000))*10**6) + " us\nF_100 avg: " + str(np.average(data.f_100l)*(1/(16000000))*10**6) + \
    "us\n\nINV avg: " + str(np.average(data.invl)*(1/(16000000))*10**6) + \
    "us\n\nS avg: " + str(np.average(data.sl)*(1/(16000000))*10**6) + "us")

    fields   = np.matrix(['R_', 'R_5', 'R_50', 'R_100', 'F_', 'F_5', 'F_50', 'F_100', 'INV', 'S'])
    complete = np.matrix([data.r_l, data.r_5l, data.r_50l, data.r_100l, data.f_l, data.f_5l, data.f_50l, data.f_100l, data.invl, data.sl])
    with open("drive_" + writefile + "np.csv", "a") as analysis:
#        analysis.write(str(fields) + "\n")
        np.savetxt(analysis, np.hstack(fields), delimiter=",", fmt="%s")
        np.savetxt(analysis, np.transpose(complete), delimiter=",", fmt='%1.0f')

        

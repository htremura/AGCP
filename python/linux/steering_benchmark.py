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

    vfl    =   np.array([])
    vffl =   np.array([])
    vml    =   np.array([])
    vsl =   np.array([])
    vc200l =   np.array([])
    vc1200l =   np.array([])

    invl =   np.array([])

    sl    =   np.array([])

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=57600, timeout=.1)
writefile = input("Which file should be written?")
count = int(input("How many runs should we try?"))
time.sleep(2)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    data1 = arduino.readline()
    return data1.decode("utf-8")
try:
    for x in range(count):
        TR = write_read("TR=")
        data.rl = np.append(data.rl, int(TR))
        time.sleep(0.15)
        S = int(write_read("S="))
        time.sleep(0.05)

        TR5 = write_read("TR5=")
        data.r5l = np.append(data.r5l, int(TR5))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        TR50 = write_read("TR50=")
        data.r50l = np.append(data.r50l, int(TR50))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        TR100 = write_read("TR100=")
        data.r100l = np.append(data.r100l, int(TR100))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        TL = write_read("TL=")
        data.ll = np.append(data.ll, int(TL))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        TL5 = write_read("TL5=")
        data.l5l = np.append(data.l5l, int(TL5))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        TL50 = write_read("TL50=")
        data.l50l = np.append(data.l50l, int(TL50))
        time.sleep(0.15)
        S += int(write_read("S="))
        time.sleep(0.05)

        TL100 = write_read("TL100=")
        data.l100l = np.append(data.l100l, int(TL100))
        time.sleep(0.15)
        S += int(write_read("S="))
        data.sl = np.append(data.sl, S/8)
        time.sleep(0.05)

        INV = write_read("INV=")
        data.invl = np.append(data.invl, int(INV))
        time.sleep(0.05)

        VF = write_read("VF=")
        data.vfl = np.append(data.vfl, int(VF))
        time.sleep(0.05)

        VFF = write_read("VFF=")
        data.vffl = np.append(data.vffl, int(VFF))
        time.sleep(0.05)

        VM = write_read("VM=")
        data.vml = np.append(data.vml, int(VM))
        time.sleep(0.05)

        VS = write_read("VS=")
        data.vsl = np.append(data.vsl, int(VS))
        time.sleep(0.05)

        VC200 = write_read("VC200=")
        data.vc200l = np.append(data.vc200l, int(VC200))
        time.sleep(0.05)

        VC1200 = write_read("VC1200=")
        data.vc1200l = np.append(data.vc1200l, int(VC1200))
        time.sleep(0.05)
except KeyboardInterrupt:
    pass
finally:

    t = open("steer_" + writefile + "a.txt", "w+")
    t.write(\
    "TR avg: " + str(np.average(data.rl)*(1/(16000000))*10**6) + " us\nTR5 avg: " + str(np.average(data.r5l)*(1/(16000000))*10**6) + "us\nTR50 avg: " + str(np.average(data.r50l)*(1/(16000000))*10**6) + " us\nTR100 avg: " + str(np.average(data.r100l)*(1/(16000000))*10**6) + \
    "us\n\nTL avg: " + str(np.average(data.ll)*(1/(16000000))*10**6) + "us\nTL5 avg: " + str(np.average(data.l5l)*(1/(16000000))*10**6) + "us\nTL50 avg: " + str(np.average(data.l50l)*(1/(16000000))*10**6) + " us\nTR100 avg: " + str(np.average(data.r100l)*(1/(16000000))*10**6) + \
    "us\n\nINV avg: " + str(np.average(data.invl)*(1/(16000000))*10**6) + \
    "us\n\nVF avg: " + str(np.average(data.vfl)*(1/(16000000))*10**6) + "us\nVFF avg: " + str(np.average(data.vffl)*(1/(16000000))*10**6) + " us\nVM avg: " + str(np.average(data.vml)*(1/(16000000))*10**6) + "us\nVS avg: " + str(np.average(data.vsl)*(1/(16000000))*10**6) + "us\nVC200 avg: " + str(np.average(data.vc200l)*(1/(16000000))*10**6) + " us\nVC1200 avg: " + str(np.average(data.vc1200l)*(1/(16000000))*10**6) + \
    "us\n\nS avg: " + str(np.average(data.sl)*(1/(16000000))*10**6) + "us")

    fields   = np.matrix(['TR', 'TR5', 'TR50', 'TR100', 'TL', 'TL5', 'TL50', 'TL100', 'INV', 'VF', 'VFF', 'VM', 'VS', 'VC200', 'VC1200', 'S'])
    complete = np.matrix([data.rl, data.r5l, data.r50l, data.r100l, data.ll, data.l5l, data.l50l, data.r100l, data.invl, data.vfl, data.vffl, data.vml, data.vsl, data.vc200l, data.vc1200l, data.sl])
    with open("steer_" + writefile + "np.csv", "a") as analysis:
#        analysis.write(str(fields) + "\n")
        np.savetxt(analysis, np.hstack(fields), delimiter=",", fmt="%s")
        np.savetxt(analysis, np.transpose(complete), delimiter=",", fmt='%1.0f')

        

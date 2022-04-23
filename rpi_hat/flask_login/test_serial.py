# -*- coding: utf-8 -*-
# use with python3
import platform
import time
import sys


def MySerialCallback():
    print("callback called!")

    
try:
    import distro
    distname = distro.like()
    print ("OS distro: " + distname)
except ImportError:
    (distname, version, nid) = platform.linux_distribution(full_distribution_name=1)
    print ("OS distro: " + distname)
    pass

if distname == 'debian':
    from gpios import *
    from serialcomm import SerialComm
    GpiosInit()
    ser = SerialComm(MySerialCallback, '/dev/serial0')
elif distname == 'Slackware ':
    from gpios_mock import *
    from serialcomm_mock import SerialComm
    GpiosInit()
    ser = SerialComm(MySerialCallback, '/dev/ttyACM0')
else:
    print ("No disname find! Closing Testing")
    sys.exit(-1)

    
if ser.port_open == False:
    print ("Port Not Open!!!")
    sys.exit(-1)
else:
    print("Serial port open OK!")

        
def TestSerialWrite():
    print ("Test sending 10 keepalives")
    for i in range(10):
        SW_TxOn()
        data_to_send = "keepalive\n"
        time_for_send = len(data_to_send)
        ser.Write(data_to_send)
        time.sleep(time_for_send / 1000)
        SW_TxOff()

        time.sleep(1)

    ser.Close()
    time.sleep(0.1)
    print ("Test end")
    time.sleep(0.1)
        
        
    
        
##############
# Main Tests #
##############
TestSerialWrite()




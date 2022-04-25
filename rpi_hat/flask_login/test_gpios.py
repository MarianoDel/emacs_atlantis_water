# -*- coding: utf-8 -*-
# use with python3
import platform
import time

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
    GpiosInit()
else:
    from gpios_mock import *
    GpiosInit()


def TestServer():
    print ("start blinking server led for 10 secs")
    LedServerToggleContinous('start')
    time.sleep(10)
    print ("ending toggling")
    LedServerToggleContinous('stop')
    print ("test ended!")


def TestLink():
    print ("start blinking link led for 10 secs")
    LedLinkToggleContinous('start')
    time.sleep(10)
    print ("ending toggling")
    LedLinkToggleContinous('stop')
    print ("test ended!")


def TestGpio17():
    print ("Test gpio17 20 sequences")
    for i in range(20):
        SW_TxOn()
        time.sleep(0.001)
        SW_TxOff()
        time.sleep(0.001)

    print ("ending toggling, test ended")


    
##############
# Main Tests #
##############
TestServer()
TestLink()
TestGpio17()





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
    

    
##############
# Main Tests #
##############
TestServer()
TestLink()





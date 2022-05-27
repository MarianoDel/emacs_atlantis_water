# -*- coding: utf-8 -*-
# use with python3

def GetDistroName ():
    try:
        import distro
        distname = distro.like()
        print ("OS distro: " + distname)
    except ImportError:
        import platform
        (distname, version, nid) = platform.linux_distribution(full_distribution_name=1)
        print ("OS distro: " + distname)

    return distname

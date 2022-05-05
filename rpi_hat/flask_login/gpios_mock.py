# -*- coding: utf-8 -*-
# use with python3
import threading
import time

############################################
# Initial Settings of all the GPIOs to use #
############################################
SW_RX_TX = 17
LED_SERVER = 27
LED_LINK = 22

# for testing outputs
SW_CH1 = 18
SW_CH2 = 19
SW_CH3 = 20
SW_CH4 = 21

def GpiosInit():
    print ("starting mocked gpios: {}, {}, {}".format(SW_RX_TX, LED_SERVER, LED_LINK))
    print ("starting mocked gpios: {}, {}, {}, {}".format(SW_CH1, SW_CH2, SW_CH3, SW_CH4))    


def GpiosCleanUp():
    print ("gpios cleanup")

    
##############
# LED Server #
##############
led_server_status = 0
def LedServerOn():
    led_server_status = 1
    print ("LED_SERVER: High")


def LedServerOff():
    led_server_status = 0
    print ("LED_SERVER: Low")


def LedServerToggle():
    if led_server_status == 0:
        LedServerOn()
    else:
        LedServerOff()

        
server_already_toggling = 0
tserver = threading.Thread()
def LedServerToggleContinous(action):
    global server_already_toggling
    global tserver

    if action == 'start':
        if not server_already_toggling:
            server_already_toggling = 1
            tserver = threading.Thread(target=LedServerToggle_Thread, args=())
            tserver.start()
            
    elif action == 'stop':
        if server_already_toggling:
            tserver.do_run = False
            LedServerOff()
            server_already_toggling = 0
            tserver.join()


def LedServerToggle_Thread():
    tserver = threading.currentThread()
    while getattr(tserver, "do_run", True):
        LedServerOn()
        time.sleep(1.3)
        LedServerOff()
        time.sleep(0.7)


############
# LED Link #
############
led_link_status = 0
def LedLinkOn():
    led_link_status = 1
    print ("LED_LINK: High")


def LedLinkOff():
    led_link_status = 0
    print ("LED_LINK: Low")


def LedLinkToggle():
    if led_link_status == 0:
        LedLinkOn()
    else:
        LedLinkOff()


tplink = threading.Thread()
def LedLinkPulse():
    global tplink

    if led_link_status == 0:
        LedLinkOn()
        tplink = threading.Timer(interval=0.1,function=LedLinkOff)
        tplink.start()
    else:
        LedLinkOff()
        tplink = threading.Timer(interval=0.1,function=LedLinkOn)
        tplink.start()

        
link_already_toggling = 0
tlink = threading.Thread()
def LedLinkToggleContinous(action):
    global link_already_toggling
    global tlink

    if action == 'start':
        if not link_already_toggling:
            link_already_toggling = 1
            tlink = threading.Thread(target=LedLinkToggle_Thread, args=())
            tlink.start()
            
    elif action == 'stop':
        if link_already_toggling:
            tlink.do_run = False
            LedLinkOff()
            link_already_toggling = 0
            tlink.join()


def LedLinkToggle_Thread():
    tlink = threading.currentThread()
    while getattr(tlink, "do_run", True):
        LedLinkOn()
        time.sleep(1.3)
        LedLinkOff()
        time.sleep(0.7)
        
        
############
# SW_RX_TX #
############
def SW_TxOn():
    print ("SW_RX_TX: High")


def SW_TxOff():
    print ("SW_RX_TX: Low")


def SW_RxOn():
    SW_TxOff()

    
############################
# Channels Switch Function #
############################
def SW_Ch1On():
    print("SW_CH1: Low")

def SW_Ch1Off():
    print("SW_CH1: High")    

    
def SW_Ch2On():
    print("SW_CH2: Low")    

def SW_Ch2Off():
    print("SW_CH2: High")        

    
def SW_Ch3On():
    print("SW_CH3: Low")    

def SW_Ch3Off():
    print("SW_CH3: High")        

    
def SW_Ch4On():
    print("SW_CH4: Low")    

def SW_Ch4Off():
    print("SW_CH4: High")        
    

### end of file ###


    

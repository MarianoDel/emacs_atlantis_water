# -*- coding: utf-8 -*-
#usar python3
import RPi.GPIO as GPIO
import threading
import time

############################################
# Initial Settings of all the GPIOs to use #
############################################
SW_RX_TX = 17
LED_SERVER = 27
LED_LINK = 22

def GpiosInit():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(SW_RX_TX, GPIO.OUT)
    GPIO.setup(LED_SERVER, GPIO.OUT)
    GPIO.setup(LED_LINK, GPIO.OUT)


def GpiosCleanUp():
    GPIO.cleanup()

    
##############
# LED Server #
##############
def LedServerOn():
    GPIO.output(LED_SERVER, GPIO.HIGH)


def LedServerOff():
    GPIO.output(LED_SERVER, GPIO.LOW)


def LedServerToggle():
    if GPIO.input(LED_SERVER) == 0:
        GPIO.output(LED_SERVER, GPIO.HIGH)
    else:
        GPIO.output(LED_SERVER, GPIO.LOW)

        
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
tplink = threading.Thread()

def LedLinkOn():
    global tplink

    if tplink.is_alive():
        tplink.cancel()
        
    GPIO.output(LED_LINK, GPIO.HIGH)


def LedLinkOff():
    global tplink

    if tplink.is_alive():
        tplink.cancel()

    GPIO.output(LED_LINK, GPIO.LOW)


def LedLinkToggle():
    global tplink

    if tplink.is_alive():
        tplink.cancel()

    if GPIO.input(LED_LINK) == 0:
        GPIO.output(LED_LINK, GPIO.HIGH)
    else:
        GPIO.output(LED_LINK, GPIO.LOW)


def LedLinkPulse():
    global tplink

    if tplink.is_alive():
        return

    if GPIO.input(LED_LINK) == 0:
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
    GPIO.output(SW_RX_TX, GPIO.HIGH)


def SW_TxOff():
    GPIO.output(SW_RX_TX, GPIO.LOW)


def SW_RxOn():
    SW_TxOff()

    
    

### end of file ###


    

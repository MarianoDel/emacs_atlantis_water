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

# for testing outputs
SW_CH1 = 26
SW_CH2 = 19
SW_CH3 = 13
SW_CH4 = 6


def GpiosInit():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(SW_RX_TX, GPIO.OUT)
    GPIO.setup(LED_SERVER, GPIO.OUT)
    GPIO.setup(LED_LINK, GPIO.OUT)
    
    GPIO.setup(SW_CH1, GPIO.OUT)
    GPIO.setup(SW_CH2, GPIO.OUT)
    GPIO.setup(SW_CH3, GPIO.OUT)
    GPIO.setup(SW_CH4, GPIO.OUT)

    GPIO.output(SW_CH1, GPIO.HIGH)
    GPIO.output(SW_CH2, GPIO.HIGH)
    GPIO.output(SW_CH3, GPIO.HIGH)
    GPIO.output(SW_CH4, GPIO.HIGH)    


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

        
        
############
# SW_RX_TX #
############
def SW_TxOn():
    GPIO.output(SW_RX_TX, GPIO.HIGH)


def SW_TxOff():
    GPIO.output(SW_RX_TX, GPIO.LOW)


def SW_RxOn():
    SW_TxOff()


############################
# Channels Switch Function #
############################
def SW_Ch1On():
    GPIO.output(SW_CH1, GPIO.LOW)

def SW_Ch1Off():
    GPIO.output(SW_CH1, GPIO.HIGH)

    
def SW_Ch2On():
    GPIO.output(SW_CH2, GPIO.LOW)

def SW_Ch2Off():
    GPIO.output(SW_CH2, GPIO.HIGH)

    
def SW_Ch3On():
    GPIO.output(SW_CH3, GPIO.LOW)

def SW_Ch3Off():
    GPIO.output(SW_CH3, GPIO.HIGH)

    
def SW_Ch4On():
    GPIO.output(SW_CH4, GPIO.LOW)

def SW_Ch4Off():
    GPIO.output(SW_CH4, GPIO.HIGH)
    
    
    

### end of file ###


    

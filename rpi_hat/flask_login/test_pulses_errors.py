# -*- coding: utf-8 -*-
# use with python3
import platform
import time
import sys
import os
import threading


###########
# Classes #
###########
class Packet_Info:
    def __init__(self, sequence=0, pulse=0, channel=0):
        self.sequence = sequence
        self.pulse = pulse
        self.channel = channel


###########
# Globals #
###########
answer_ok = False
link_up = False
last_sequence = -1
meas_channel = [0, 0, 0, 0, 0]    # five index vector from 1 to 4
keeps = 0
okeys = 0
noks = 0
bad_pckt = 0
link_up_tx_timeout = 0
link_up_rx_timeout = 0
link_up_led = False
tt = 0

#############
# Functions #
#############
def MySerialCallback(to_clean_data):
    global answer_ok
    global link_up
    global keeps
    global okeys
    global noks
    global bad_pckt

    data_rx = ''
    for i in range(len(to_clean_data)):
        if ord(to_clean_data[i]) != 0:
            data_rx += to_clean_data[i]
            
    if data_rx.startswith("keepalive"):
        answer_ok = True
        # link_up = True
        keeps += 1
    elif data_rx.startswith("s"):
        ans = process_packet(data_rx)
        if ans == 1:
            answer_ok = True
            link_up = True

    elif data_rx.startswith("ok"):
        link_up = True
        okeys += 1
    elif data_rx.startswith("nok"):
        # link_up = True
        noks += 1        
    else:
        print("bad packet, no process")
        bad_pckt += 1


def process_packet (data):
    global last_sequence
    global meas_channel
    global send_ack
    
    pkt = Packet_Info()

    if check_packet_info_from_string(data, pkt) == 0:
        print("bad sequence packet, no process")
        return 0

    if pkt.sequence == 0 or \
       pkt.sequence <= last_sequence:
        print("bad sequence number, no process")
        return 0
    
    meas_channel[pkt.channel] += pkt.pulses
    last_sequence = pkt.sequence
    return 1


        
def check_packet_info_from_string (data, pkt):
    sequence = 0
    pulses = 0
    channel = 0

    # only str or str+\n
    if len(data) < 19 or len(data) > 20:
        print("bad length")
        return 0
    
    try:
        sequence = int(data[1:4])
    except:
        print("bad sequence str")
        return 0

    if data[4:12] != ' pulses ':
        print("bad pulses str: " + data[4:12])
        return 0

    try:
        pulses = int(data[12:15])
    except:
        print("bad pulses number: " + data[12:15])
        return 0

    channel = data[16:19]
    if channel == 'ch1':
        channel = 1
    elif channel == 'ch2':
        channel = 2        
    elif channel == 'ch3':
        channel = 3        
    elif channel == 'ch4':
        channel = 4
    else:
        print("bad channel number: " + data[16:])
        return 0                
    
    pkt.sequence = sequence
    pkt.pulses = pulses
    pkt.channel = channel
    return 1
    
    
    
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
        time.sleep(0.001)
        data_to_send = "keepalive\n"
        time_for_send = len(data_to_send) + 2
        ser.Write(data_to_send)
        time.sleep(time_for_send / 1000)
        SW_TxOff()

        time.sleep(1)

    ser.Close()
    time.sleep(0.1)
    print ("Test end")
    time.sleep(0.1)


def display_title_bar (port_state=False, link_up=False):
    # Clears the terminal screen, and displays a title bar.
    os.system('clear')
    if port_state:
        port_state = "Open"
    else:
        port_state = "Close"

    if link_up:
        link_up = "Up"
    else:
        link_up = "Down"
        
    print("\t**********************************************")
    print("\t***    KIRNO - Atlantis Pulses Test -      ***")
    print("\t**********************************************")
    print("\t[1] Send pulse ch1.")
    print("\t[2] Link UP.")
    print("\t[3] Link Down.")
    print("\t[4] Get Manufacturer ID.")
    print("\t**********************************************")
    print("\t* Port: {}\tLink: {}\t\t     *".format(port_state, link_up))
    print("\t**********************************************")    
    print("\n")


def get_user_choice():
    global link_up
    # Let users know what they can do.
    choice = input(" your choice: ")
    if choice == '1':
        pass
    elif choice == '2':
        link_up = True
    elif choice == '3':
        link_up = False
    elif choice == 'q':
        print("\nThanks for playing. Bye.")
        return False
    else:
        print("\nI didn't understand that choice.\n")
    
    return True

# def StartRx ():
#     hilo1 = threading.Thread(target=get_user_choice, args=())
#     hilo1.start()


def transmission (data_to_send):
    t_tx = threading.Thread(target=transmission_thread, args=(ser, data_to_send))
    t_tx.start()


def transmission_thread (sport, data_to_send):
    SW_TxOn()
    LedLinkPulse()
    time.sleep(0.001)
    time_for_send = len(data_to_send) + 2
    sport.Write(data_to_send)
    time.sleep(time_for_send / 1000)
    SW_TxOff()

    
def TestBoard ():
    global link_up
    
    last_link = link_up
    no_end = True
    
    while (no_end):
        display_title_bar(ser.port_open, link_up)
        no_end = get_user_choice()

    ser.Close()
    

def TestLinkUp ():
    global link_up
    global answer_ok
    global link_up_tx_timeout
    global link_up_rx_timeout    
    global link_up_led
    global tt

    tt = threading.Timer(interval=0.1, function=ModuleTimeouts)
    tt.start()

    link_up_led = False
    LedLinkOff()

    last_keep = 0
    last_okeys = 0
    last_noks = 0
    last_bad_pckt = 0
    while True:
        if last_keep != keeps or last_okeys != okeys or last_noks != noks or last_bad_pckt != bad_pckt:
            good = keeps + okeys
            error = noks + bad_pckt
            total = good + error
            perc = error / total * 100
            print(f'keeps: {keeps} ok: {okeys} nok: {noks} bad_pckt: {bad_pckt} rate: {perc}%')
            last_keep = keeps
            last_okeys = okeys
            last_noks = noks
            last_bad_pckt = bad_pckt

        # receiv 'ok' to a 'keepalive' 
        if link_up:
            link_up = False
            link_up_rx_timeout = 50
            if link_up_led == False:
                link_up_led = True
                LedLinkOn()
            
        # answer 'ok' to a keepalive asking
        if answer_ok:
            answer_ok = False
            time.sleep(0.005)
            transmission('ok\n')

        if link_up_rx_timeout == 0:
            if link_up_led:
                link_up_led = False
                LedLinkOff()

        if link_up_tx_timeout == 0:
            transmission('keepalive\n')
            link_up_tx_timeout = 15
            
        time.sleep(0.001)
    

def ModuleTimeouts ():
    global link_up_tx_timeout
    global link_up_rx_timeout
    global tt
    
    if link_up_tx_timeout > 0:
        link_up_tx_timeout -= 1

    if link_up_rx_timeout > 0:
        link_up_rx_timeout -= 1
        
    tt = threading.Timer(interval=0.1, function=ModuleTimeouts)    
    tt.start()

        
def TestCb ():
    # fixed msgs
    for i in range(1000):
        seq = i
        seq_msg = 's{0:03d}'.format(seq)
        pulses = 1
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' pulses ' + pulses_msg + ' ch1'
        print("")
        print(message)
        pi = Packet_Info()
        check_packet_info_from_string(message, pi)
        if pi.sequence == 0:
            print("ERROR!!! no packet valid data")
        else:
            print("seq: {} pulses: {} channel: {}".format(pi.sequence, pi.pulses, pi.channel))


    for i in range(1000):
        seq = 1
        seq_msg = 's{0:03d}'.format(seq)
        pulses = i
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' pulses ' + pulses_msg + ' ch1'
        print("")
        print(message)
        pi = Packet_Info()
        check_packet_info_from_string(message, pi)
        if pi.sequence == 0:
            print("ERROR!!! no packet valid data")
        else:
            print("seq: {} pulses: {} channel: {}".format(pi.sequence, pi.pulses, pi.channel))


    for i in range(1,5):
        seq = 999
        seq_msg = 's{0:03d}'.format(seq)
        pulses = 999
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' pulses ' + pulses_msg + ' ch' + str(i)
        print("")
        print(message)
        pi = Packet_Info()
        check_packet_info_from_string(message, pi)
        if pi.sequence == 0:
            print("ERROR!!! no packet valid data")
        else:
            print("seq: {} pulses: {} channel: {}".format(pi.sequence, pi.pulses, pi.channel))


    

        
##############
# Main Tests #
##############
# TestSerialWrite()
# TestCb()
# TestBoard()
TestLinkUp()




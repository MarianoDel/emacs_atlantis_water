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


class Statics_Info:
    def __init__(self, keeps=0, okeys=0, noks=0, bad_pckt=0):
        self.keeps = keeps
        self.okeys = okeys
        self.noks = noks
        self.bad_pckt = bad_pckt
        

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
key_choice = ''
tp1 = threading.Thread()
tp2 = threading.Thread()
tp3 = threading.Thread()
tp4 = threading.Thread()

init_time = time.time()


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

    if len(data_rx) > 0:
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
            # print("bad packet, no process")
            # print("bad packet len: " + str(len(data_rx)) + " data: " + data_rx)
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
    ser = SerialComm(MySerialCallback, '/dev/serial0',show_rx=False)
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
    # global meas_channel
    # global keeps
    # global okeys
    # global noks
    # global bad_pckt
    
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

    total = keeps + okeys + bad_pckt + noks
    error = bad_pckt + noks
    perc = 0
    
    if total != 0:
        perc = error / total * 100

    current_time = time.time()
    elapsed_time = current_time - init_time
    pps = total / elapsed_time
    
        
    print("\t**********************************************")
    print("\t***    KIRNO - Atlantis Pulses Test -      ***")
    print("\t**********************************************")
    print("\t[1] Send pulse ch1.")
    print("\t[2] Send pulse ch2.")
    print("\t[3] Send pulse ch3.")
    print("\t[4] Send pulse ch4.")
    print("\t**********************************************")
    print("\t* Port: {}\tLink: {}\tElapsed: {:.2f}\t  *".format(port_state, link_up, elapsed_time))
    print("\t**********************************************")
    print(f'\t* keeps: {keeps} ok: {okeys} nok: {noks} bad_pckt: {bad_pckt}')
    print(f'\t* rate: {perc:02.02f}%')
    print(f'\t* packet per second: {pps:.02f}')    
    print("\t**********************************************")
    print(f'\t*\tch1: {meas_channel[1]}\tch2: {meas_channel[2]}\tch3: {meas_channel[3]}\tch4: {meas_channel[4]}\t     *')
    print("\t**********************************************")    
    print("")


def get_user_choice():
    global key_choice

    loop = True
    while loop:
        # Let users know what they can do.
        choice = input(" your choice: ")
        key_choice = choice
        if choice == 'q':
            loop = False
            

def start_key_input ():
    hilo1 = threading.Thread(target=get_user_choice, args=())
    hilo1.start()


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


def pulse_ch1 (timer_in_on):
    in_secs = timer_in_on/1000
    SW_Ch1On()
    time.sleep(in_secs)
    SW_Ch1Off()
    time.sleep(in_secs)    

def pulse_ch2 (timer_in_on):
    in_secs = timer_in_on/1000    
    SW_Ch2On()
    time.sleep(in_secs)    
    SW_Ch2Off()
    time.sleep(in_secs)    

def pulse_ch3 (timer_in_on):
    in_secs = timer_in_on/1000    
    SW_Ch3On()
    time.sleep(in_secs)    
    SW_Ch3Off()
    time.sleep(in_secs)    

def pulse_ch4 (timer_in_on):
    in_secs = timer_in_on/1000    
    SW_Ch4On()
    time.sleep(in_secs)    
    SW_Ch4Off()
    time.sleep(in_secs)    
    
    
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
    global key_choice
    global tp1
    global tp2
    global tp3
    global tp4
    

    tt = threading.Timer(interval=0.1, function=ModuleTimeouts)
    tt.start()

    link_up_led = False
    LedLinkOff()
    update_chart = 0

    display_title_bar()
    start_key_input()
    
    while True:
        # show updated chart
        if update_chart == 0:
            update_chart = 3
            display_title_bar(ser.port_open, link_up_led)


        if key_choice == 'q':
            print("closing app")
            return
        elif key_choice == '1' and tp1.is_alive() == False:
            tp1 = threading.Thread(target=pulse_ch1, args=(200,))
            tp1.start()
            key_choice = ''
        elif key_choice == '2' and tp2.is_alive() == False:
            tp2 = threading.Thread(target=pulse_ch2, args=(200,))
            tp2.start()
            key_choice = ''            
        elif key_choice == '3' and tp3.is_alive() == False:
            tp3 = threading.Thread(target=pulse_ch3, args=(200,))
            tp3.start()
            key_choice = ''            
        elif key_choice == '4' and tp4.is_alive() == False:
            tp4 = threading.Thread(target=pulse_ch4, args=(200,))
            tp4.start()
            key_choice = ''            
            

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
            link_up_tx_timeout = 7
            time.sleep(0.005)
            transmission('ok\n')

        if link_up_rx_timeout == 0:
            if link_up_led:
                link_up_led = False
                LedLinkOff()

        if link_up_tx_timeout == 0:
            transmission('keepalive\n')
            link_up_tx_timeout = 15
            if update_chart:
                update_chart -= 1
            
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




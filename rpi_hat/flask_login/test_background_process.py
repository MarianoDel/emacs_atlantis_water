# -*- coding: utf-8 -*-
# use with python3
import platform
import time
import sys
import os
import threading
import random


## OS where its run
from get_distroname import GetDistroName
distname = GetDistroName()
if distname == 'debian':
    RUNNING_ON_SLACKWARE = 0
    RUNNING_ON_RASP = 1
    SIMULATE_PULSES = False    
elif distname == 'Slackware ':
    RUNNING_ON_SLACKWARE = 1
    RUNNING_ON_RASP = 0
    SIMULATE_PULSES = True
else:
    print('No distro finded!')
    exit(-1)

######################
# Background Process #
######################
from background_process import MeterProcess
my_proc = None
def bkg_process ():
    global my_proc

    if SIMULATE_PULSES:
        mtt = threading.Timer(interval=2, function=bkg_test)
        mtt.start()
    
    my_proc = MeterProcess(bkg_new_meas)
    t1 = threading.Thread(target=bkg_handler)
    t1.start()
    # t1.join()


def bkg_handler ():
    global my_proc
    
    while True:
        my_proc.CommsProcess()
        

def bkg_new_meas (meas_list):
    print("  meas list: " + str(meas_list))


myseq = 0
mytest_state = 'init'
def bkg_test ():
    global my_proc
    global mytest_state
    global myseq

    # send data to serial_mock and recall
    if mytest_state == 'init':
        mytest_state = 'send_p1'
        
    elif mytest_state == 'send_p1':
        seq_msg = 's{0:03d}'.format(myseq)
        pulses = 1
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' 0000'
        my_proc.MySerialCallback(message)
        if myseq < 999:
            myseq += 1
        else:
            myseq = 1
            
        mytest_state = 'send_p2'
        
    elif mytest_state == 'send_p2':
        seq_msg = 's{0:03d}'.format(myseq)
        pulses = 0
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' 0000'
        my_proc.MySerialCallback(message)
        if myseq < 999:
            myseq += 1
        else:
            myseq = 1

        mytest_state = 'send_p3'

    elif mytest_state == 'send_p3':
        seq_msg = 's{0:03d}'.format(myseq)
        pulses = 2
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' ' + pulses_msg + ' 0008'
        my_proc.MySerialCallback(message)
        if myseq < 999:
            myseq += 1
        else:
            myseq = 1

        mytest_state = 'send_p4'

    elif mytest_state == 'send_p4':
        seq_msg = 's{0:03d}'.format(myseq)
        pulses_1 = random.randrange(0,999)
        pulses_2 = random.randrange(0,999)
        pulses_3 = random.randrange(0,999)
        pulses_4 = random.randrange(0,999)
        pulses_ch = pulses_1 + pulses_2 + pulses_3 + pulses_4
        p1 = '{0:03d}'.format(pulses_1)
        p2 = '{0:03d}'.format(pulses_2)
        p3 = '{0:03d}'.format(pulses_3)
        p4 = '{0:03d}'.format(pulses_4)
        pc = '{0:04d}'.format(pulses_ch)        
        message = seq_msg + ' ' + p1 + ' ' + p2 + ' ' + p3 + ' ' + p4 + ' ' + pc
        my_proc.MySerialCallback(message)
        if myseq < 999:
            myseq += 1
        else:
            myseq = 1

        mytest_state = 'init'
        
    else:
        mytest_state = 'init'
        
        
    # mtt = threading.Timer(interval=0.5, function=myTestTimeout)
    mtt = threading.Timer(interval=4, function=bkg_test)
    mtt.start()

    
###########
# Classes #
###########
# class Packet_Info:
#     def __init__(self, sequence=0, pulse=0, channel=0):
#         self.sequence = sequence
#         self.pulse = pulse
#         self.channel = channel


# class Statics_Info:
#     def __init__(self, keeps=0, okeys=0, noks=0, bad_pckt=0):
#         self.keeps = keeps
#         self.okeys = okeys
#         self.noks = noks
#         self.bad_pckt = bad_pckt
        

# ###########
# # Globals #
# ###########
# answer_ok = False
# link_up = False
# last_sequence = -1
# last_getted = -1
# meas_channel = [0, 0, 0, 0, 0]    # five index vector from 1 to 4
# gen_channel = [0, 0, 0, 0, 0]    # five index vector from 1 to 4
# keeps = 0
# okeys = 0
# noks = 0
# bad_pckt = 0
# link_up_tx_timeout = 0
# link_up_rx_timeout = 0
# link_up_led = False
# tt = 0
# tp1 = threading.Thread()
# tp2 = threading.Thread()
# tp3 = threading.Thread()
# tp4 = threading.Thread()

# ch1_timeout = 0
# ch2_timeout = 0
# ch3_timeout = 0
# ch4_timeout = 0    

# init_time = time.time()


# #############
# # Functions #
# #############
# def MySerialCallback(to_clean_data):
#     global answer_ok
#     global link_up
#     global keeps
#     global okeys
#     global noks
#     global bad_pckt

#     data_rx = ''
#     for i in range(len(to_clean_data)):
#         if ord(to_clean_data[i]) != 0:
#             data_rx += to_clean_data[i]

#     if len(data_rx) > 0:
#         if data_rx.startswith("keepalive"):
#             answer_ok = True
#             # link_up = True
#             keeps += 1
#         elif data_rx.startswith("s"):
#             ans = process_packet(data_rx)
#             if ans == 1:
#                 answer_ok = True
#                 link_up = True

#         elif data_rx.startswith("ok"):
#             link_up = True
#             okeys += 1
#         elif data_rx.startswith("nok"):
#             # link_up = True
#             noks += 1        
#         else:
#             # print("bad packet, no process")
#             # print("bad packet len: " + str(len(data_rx)) + " data: " + data_rx)
#             bad_pckt += 1


# def process_packet (data):
#     global last_sequence
#     global last_getted    
#     global meas_channel
#     global send_ack
    
#     pkt = Packet_Info()

#     if check_packet_info_from_string(data, pkt) == 0:
#         print("bad sequence packet, no process")
#         return 0

#     last_getted = pkt.sequence
#     if pkt.sequence == 0 or \
#        pkt.sequence <= last_sequence:
#         print("bad sequence number, no process")
#         return 0
    
#     meas_channel[pkt.channel] += pkt.pulses
#     if pkt.sequence != 999:
#         last_sequence = pkt.sequence
#     else:
#         last_sequence = 0
        
#     return 1


        
# def check_packet_info_from_string (data, pkt):
#     sequence = 0
#     pulses = 0
#     channel = 0

#     # only str or str+\n
#     if len(data) < 19 or len(data) > 20:
#         print("bad length")
#         return 0
    
#     try:
#         sequence = int(data[1:4])
#     except:
#         print("bad sequence str")
#         return 0

#     if data[4:12] != ' pulses ':
#         print("bad pulses str: " + data[4:12])
#         return 0

#     try:
#         pulses = int(data[12:15])
#     except:
#         print("bad pulses number: " + data[12:15])
#         return 0

#     channel = data[16:19]
#     if channel == 'ch1':
#         channel = 1
#     elif channel == 'ch2':
#         channel = 2        
#     elif channel == 'ch3':
#         channel = 3        
#     elif channel == 'ch4':
#         channel = 4
#     else:
#         print("bad channel number: " + data[16:])
#         return 0                
    
#     pkt.sequence = sequence
#     pkt.pulses = pulses
#     pkt.channel = channel
#     return 1
    
    
    
# try:
#     import distro
#     distname = distro.like()
#     print ("OS distro: " + distname)
# except ImportError:
#     (distname, version, nid) = platform.linux_distribution(full_distribution_name=1)
#     print ("OS distro: " + distname)
#     pass

# if distname == 'debian':
#     from gpios import *
#     from serialcomm import SerialComm
#     GpiosInit()
#     ser = SerialComm(MySerialCallback, '/dev/serial0',show_rx=False)
# elif distname == 'Slackware ':
#     from gpios_mock import *
#     from serialcomm_mock import SerialComm
#     GpiosInit()
#     ser = SerialComm(MySerialCallback, '/dev/ttyACM0')
# else:
#     print ("No disname find! Closing Testing")
#     sys.exit(-1)

    
# if ser.port_open == False:
#     print ("Port Not Open!!!")
#     sys.exit(-1)
# else:
#     print("Serial port open OK!")

        
# def TestSerialWrite():
#     print ("Test sending 10 keepalives")
#     for i in range(10):
#         SW_TxOn()
#         time.sleep(0.001)
#         data_to_send = "keepalive\n"
#         time_for_send = len(data_to_send) + 2
#         ser.Write(data_to_send)
#         time.sleep(time_for_send / 1000)
#         SW_TxOff()

#         time.sleep(1)

#     ser.Close()
#     time.sleep(0.1)
#     print ("Test end")
#     time.sleep(0.1)


# def display_title_bar (port_state=False, link_up=False):
#     # global meas_channel
#     # global keeps
#     # global okeys
#     # global noks
#     # global bad_pckt
    
#     # Clears the terminal screen, and displays a title bar.
#     os.system('clear')
#     if port_state:
#         port_state = "Open"
#     else:
#         port_state = "Close"

#     if link_up:
#         link_up = "Up"
#     else:
#         link_up = "Down"

#     total = keeps + okeys + bad_pckt + noks
#     error = bad_pckt + noks
#     perc = 0
    
#     if total != 0:
#         perc = error / total * 100

#     current_time = time.time()
#     elapsed_time = current_time - init_time
#     pps = total / elapsed_time
    
        
#     print("\t**********************************************")
#     print("\t***    KIRNO - Atlantis Pulses Test -      ***")
#     print("\t**********************************************")
#     print("\t* Port: {}  Link: {}  Elapsed: {:.2f}\t     *".format(port_state, link_up, elapsed_time))
#     print("\t**********************************************")
#     print(f'\t* keeps: {keeps} ok: {okeys} nok: {noks} bad_pckt: {bad_pckt}')
#     print(f'\t* rate: {perc:02.02f}%')
#     print(f'\t* packet per second: {pps:.02f}')    
#     print("\t**********************************************")
#     print(f'\t*get  ch1: {meas_channel[1]}\tch2: {meas_channel[2]}\tch3: {meas_channel[3]}\tch4: {meas_channel[4]}\t     *')
#     print("\t**********************************************")    
#     print(f'\t*sent ch1: {gen_channel[1]}\tch2: {gen_channel[2]}\tch3: {gen_channel[3]}\tch4: {gen_channel[4]}\t     *')
#     print("\t**********************************************")
#     print(f'\t*seq waited: {last_sequence}\tseq getted: {last_getted}')
#     print("\t**********************************************")    
#     print("")



# def transmission (data_to_send):
#     t_tx = threading.Thread(target=transmission_thread, args=(ser, data_to_send))
#     t_tx.start()


# def transmission_thread (sport, data_to_send):
#     SW_TxOn()
#     LedLinkPulse()
#     time.sleep(0.001)
#     time_for_send = len(data_to_send) + 2
#     sport.Write(data_to_send)
#     time.sleep(time_for_send / 1000)
#     SW_TxOff()


# def pulse_ch1 (timer_in_on):
#     in_secs = timer_in_on/10
#     SW_Ch1On()
#     time.sleep(in_secs)
#     SW_Ch1Off()
#     time.sleep(in_secs)    
    
# def pulse_ch2 (timer_in_on):
#     in_secs = timer_in_on/10
#     SW_Ch2On()
#     time.sleep(in_secs)    
#     SW_Ch2Off()
#     time.sleep(in_secs)    

# def pulse_ch3 (timer_in_on):
#     in_secs = timer_in_on/10
#     SW_Ch3On()
#     time.sleep(in_secs)    
#     SW_Ch3Off()
#     time.sleep(in_secs)    

# def pulse_ch4 (timer_in_on):
#     in_secs = timer_in_on/10
#     SW_Ch4On()
#     time.sleep(in_secs)    
#     SW_Ch4Off()
#     time.sleep(in_secs)    
    
    
# def TestBoard ():
#     global link_up
    
#     last_link = link_up
#     no_end = True
    
#     while (no_end):
#         display_title_bar(ser.port_open, link_up)
#         no_end = get_user_choice()

#     ser.Close()
    

    

# # 100ms timer
# def ModuleTimeouts ():
#     global link_up_tx_timeout
#     global link_up_rx_timeout
#     global tt
#     global ch1_timeout
#     global ch2_timeout
#     global ch3_timeout
#     global ch4_timeout    
    
#     if link_up_tx_timeout > 0:
#         link_up_tx_timeout -= 1

#     if link_up_rx_timeout > 0:
#         link_up_rx_timeout -= 1

#     if ch1_timeout > 0:
#         ch1_timeout -= 1

#     if ch2_timeout > 0:
#         ch2_timeout -= 1

#     if ch3_timeout > 0:
#         ch3_timeout -= 1

#     if ch4_timeout > 0:
#         ch4_timeout -= 1                
        
#     tt = threading.Timer(interval=0.1, function=ModuleTimeouts)    
#     tt.start()

        


    

        
##############
# Main Tests #
##############
bkg_process()





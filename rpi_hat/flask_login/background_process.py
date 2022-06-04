# -*- coding: utf-8 -*-
# use with python3
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


class MeterProcess:
    def __init__(self, cb):
        self.cb = cb

        self.answer_ok = False
        self.link_up = False
        self.keeps = 0
        self.okeys = 0
        self.noks = 0
        self.bad_pckt = 0

        self.last_sequence = -1
        self.last_getted = -1
        self.meas_channel = [0, 0, 0, 0, 0]    # five index vector from 1 to 4

        self.link_up_tx_timeout = 0
        self.link_up_rx_timeout = 0
        self.tt = 0

        self.measCallback = cb

        # start the hardware module, get the distro
        from get_distroname import GetDistroName
        distname = GetDistroName()
        if distname == 'debian':
            from gpios import GpiosInit, LedLinkOn, LedLinkOff, SW_TxOn, SW_TxOff, \
                LedLinkPulse, LedServerOn, LedServerOff, LedServerToggleContinous
            from serialcomm import SerialComm
            GpiosInit()
            SW_TxOn()
            LedLinkOff()
            
            self.SW_TxOn = SW_TxOn
            self.SW_TxOff = SW_TxOff
            self.LedLinkOn = LedLinkOn
            self.LedLinkOff = LedLinkOff            
            self.LedLinkPulse = LedLinkPulse
            self.LedServerOn = LedServerOn
            self.LedServerOff = LedServerOff
            self.LedServerToggleContinous = LedServerToggleContinous
            self.ser = SerialComm(self.MySerialCallback, '/dev/serial0',show_rx=False)
        elif distname == 'Slackware ':
            from gpios_mock import GpiosInit, LedLinkOn, LedLinkOff, SW_TxOn, SW_TxOff, \
                LedLinkPulse, LedServerOn, LedServerOff, LedServerToggleContinous
            from serialcomm_mock import SerialComm
            GpiosInit()
            SW_TxOn()
            LedLinkOff()
            
            self.SW_TxOn = SW_TxOn
            self.SW_TxOff = SW_TxOff
            self.LedLinkOn = LedLinkOn
            self.LedLinkOff = LedLinkOff
            self.LedLinkPulse = LedLinkPulse
            self.LedServerOn = LedServerOn
            self.LedServerOff = LedServerOff
            self.LedServerToggleContinous = LedServerToggleContinous            
            self.ser = SerialComm(self.MySerialCallback, '/dev/ttyACM0')
        else:
            print ("No distname find! Closing Testing")
            sys.exit(-1)

        if self.ser.port_open == False:
            print ("Port Not Open!!!")
            sys.exit(-1)

        print("Serial port open OK!")

        # init all internal funcs
        # init the 100ms timeout timer
        self.tt = threading.Timer(interval=0.1, function=self.ModuleTimeouts)
        self.tt.start()

        # init the leds state
        self.link_up_led = False
        LedLinkOff()

        # init the loop process
        print("MeterProcess initialized")

        
    def CommsProcess(self):
        # receiv 'ok' to a 'keepalive'
        if self.link_up:
            self.link_up = False
            self.link_up_rx_timeout = 50
            if self.link_up_led == False:
                self.link_up_led = True
                self.LedLinkOn()

        # answer 'ok' to a keepalive asking
        if self.answer_ok:
            self.answer_ok = False
            self.link_up_tx_timeout = 7
            time.sleep(0.005)
            self.transmission('ok\n')

        if self.link_up_rx_timeout == 0:
            if self.link_up_led:
                self.link_up_led = False
                self.LedLinkOff()
                # with link down reset the seq number
                self.last_sequence = 0

        if self.link_up_tx_timeout == 0:
            self.transmission('keepalive\n')
            self.link_up_tx_timeout = 15

        if self.meas_channel != [0, 0, 0, 0, 0]:
            self.measCallback(self.meas_channel)
            for i in range(len(self.meas_channel)):
                self.meas_channel[i] = 0

        time.sleep(0.001)


    #######################
    # Transimission Utils #
    #######################
    def transmission (self, data_to_send):
        self.t_tx = threading.Thread(target=self.transmission_thread, args=(self.ser, data_to_send))
        self.t_tx.start()


    def transmission_thread (self, sport, data_to_send):
        self.SW_TxOn()
        self.LedLinkPulse()
        time.sleep(0.001)
        time_for_send = len(data_to_send) + 2
        sport.Write(data_to_send)
        time.sleep(time_for_send / 1000)
        self.SW_TxOff()


    ##################
    # Receiver Utils #
    ##################
    def MySerialCallback(self, to_clean_data):
        data_rx = ''
        for i in range(len(to_clean_data)):
            if ord(to_clean_data[i]) != 0:
                data_rx += to_clean_data[i]

        if len(data_rx) > 0:
            if data_rx.startswith("keepalive"):
                self.answer_ok = True
                self.keeps += 1

            elif data_rx.startswith("s"):
                ans = self.process_packet(data_rx)
                if ans == 1:
                    self.answer_ok = True
                    self.link_up = True

            elif data_rx.startswith("ok"):
                self.link_up = True
                self.okeys += 1

            elif data_rx.startswith("nok"):
                # self.link_up = True
                self.noks += 1

            else:
                # print("bad packet, no process")
                # print("bad packet len: " + str(len(data_rx)) + " data: " + data_rx)
                self.bad_pckt += 1


    def process_packet (self, data):
        pkt = Packet_Info()

        if self.check_packet_info_from_string(data, pkt) == 0:
            print("bad sequence packet, no process")
            return 0

        if pkt.sequence == 0 or \
           pkt.sequence <= self.last_sequence:
            print("bad sequence number, no process")
            return 0

        self.meas_channel[pkt.channel] += pkt.pulses
        if pkt.sequence != 999:
            self.last_sequence = pkt.sequence
        else:
            self.last_sequence = 0

        return 1


    def check_packet_info_from_string (self, data, pkt):
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

    
    ###########
    # Statics #
    ###########
    def Statics_Info (self):
        return (self.keeps, self.okeys, self.noks, self.bad_pckt)
    

    ##############
    # Leds Utils #
    ##############
    def LedServer (self, status_str):
        if status_str == 'on':
            self.LedServerToggleContinous('stop')
            self.LedServerOn()
        elif status_str == 'off':
            self.LedServerToggleContinous('stop')
            self.LedServerOff()
        elif status_str == 'toggle on':
            self.LedServerToggleContinous('start')
        elif status_str == 'toggle off':
            self.LedServerToggleContinous('stop')
            
    ###############
    # Timer Utils #
    ###############
    # 100ms timer
    def ModuleTimeouts (self):
        if self.link_up_tx_timeout > 0:
            self.link_up_tx_timeout -= 1

        if self.link_up_rx_timeout > 0:
            self.link_up_rx_timeout -= 1

        self.tt = threading.Timer(interval=0.1, function=self.ModuleTimeouts)
        self.tt.start()




#####################
# Testing Functions #
#####################
my_proc = None
def TestObjects ():
    global my_proc
    
    print("Creating objects...")

    my_packet = Packet_Info()
    my_statics = Statics_Info()

    mtt = threading.Timer(interval=2, function=myTestTimeout)
    mtt.start()

    my_proc = MeterProcess(myCallback)
    print("Objects created")

    while True:
        my_proc.CommsProcess()

    
def myCallback (data):
    print ("  callback called: " + str(data))
    (k, o, n, b) = my_proc.Statics_Info()
    print (f'  statics keeps {k} okeys {o} noks {n} bads {b}')


myseq = 0
mytest_state = 'init'
def myTestTimeout ():
    global my_proc
    global mytest_state
    global myseq

    # send data to serial_mock and recall
    if mytest_state == 'init':
        my_proc.MySerialCallback('keepalive')
        mytest_state = 'send_p1'
        
    elif mytest_state == 'send_p1':
        seq_msg = 's{0:03d}'.format(myseq)
        pulses = 1
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' pulses ' + pulses_msg + ' ch1'
        my_proc.MySerialCallback(message)
        if myseq < 999:
            myseq += 1
        else:
            myseq = 1
            
        mytest_state = 'send_p2'
        
    elif mytest_state == 'send_p2':
        seq_msg = 's{0:03d}'.format(myseq)
        pulses = 1
        pulses_msg = '{0:03d}'.format(pulses)
        message = seq_msg + ' pulses ' + pulses_msg + ' ch2'
        my_proc.MySerialCallback(message)
        if myseq < 999:
            myseq += 1
        else:
            myseq = 1

        mytest_state = 'init'
        
    else:
        mytest_state = 'init'
        
        
    mtt = threading.Timer(interval=0.5, function=myTestTimeout)
    mtt.start()

    
##############
# Main Tests #
##############
if __name__ == "__main__":

    TestObjects()


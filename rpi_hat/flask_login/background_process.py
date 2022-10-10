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
        self.pulses_ch1 = pulse
        self.pulses_ch2 = pulse
        self.pulses_ch3 = pulse
        self.pulses_ch4 = pulse
        self.pulses_check = pulse


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
        self.keeps = 0
        self.okeys = 0
        self.noks = 0
        self.bad_pckt = 0

        self.last_sequence = -1
        self.last_getted = -1
        self.meas_channel = [0, 0, 0, 0, 0]    # five index vector from 1 to 4

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
        # self.link_up_led = False
        # LedLinkOff()
        self.link_up_led = True
        LedLinkOn()
        self.link_up_rx_timeout = 100

        # init the loop process
        print("MeterProcess initialized")

        
    def CommsProcess(self):
        # answer 'ok' to a meas packet asking
        if self.answer_ok:
            self.answer_ok = False
            time.sleep(0.005)
            seq_str = '{0:03d}'.format(self.answer_ok_seq)
            self.transmission('s' + seq_str + ' ok\n')

        if self.link_up_rx_timeout == 0:
            if self.link_up_led:
                self.link_up_led = False
                self.LedLinkOff()

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

        if len(data_rx) >= 25:
            print("rx len: " + str(len(data_rx)) + " data: " + str(data_rx))
            if data_rx.startswith('s'):
                ans = self.process_packet(data_rx)
                if ans == 1:
                    self.answer_ok = True
                    self.answer_ok_seq = self.last_sequence
                    self.link_up_rx_timeout = 20
                    self.link_up_led = True
                    self.LedLinkOn()


            else:
                # print("bad packet, no process")
                # print("bad packet len: " + str(len(data_rx)) + " data: " + data_rx)
                self.bad_pckt += 1


    def process_packet (self, data):
        pkt = Packet_Info()

        if self.check_packet_info_from_string(data, pkt) == 0:
            print("bad sequence packet, no process")
            return 0

        if pkt.sequence == self.last_sequence:
            print("same seq packet, no process")
            return 0

        if pkt.pulses_check != pkt.pulses_ch1 + pkt.pulses_ch2 + pkt.pulses_ch3 + pkt.pulses_ch4:
            print(str(pkt.pulses_ch1))
            print(str(pkt.pulses_ch2))
            print(str(pkt.pulses_ch3))
            print(str(pkt.pulses_ch4))            
            print(str(pkt.pulses_check))
            print("bad checksum, no process")
            return 0
        
        self.meas_channel[1] += pkt.pulses_ch1
        self.meas_channel[2] += pkt.pulses_ch2
        self.meas_channel[3] += pkt.pulses_ch3
        self.meas_channel[4] += pkt.pulses_ch4

        self.last_sequence = pkt.sequence

        return 1


    def check_packet_info_from_string (self, data, pkt):
        sequence = 0
        pulses_ch1 = 0
        pulses_ch2 = 0
        pulses_ch3 = 0
        pulses_ch4 = 0
        pulses_check = 0        

        # only str or str+\n
        if len(data) < 25 or len(data) > 26:
            print("bad length")
            return 0

        try:
            sequence = int(data[1:4])
        except:
            print("bad sequence str")
            return 0

        try:
            pulses_ch1 = int(data[5:8])
            pulses_ch2 = int(data[9:12])
            pulses_ch3 = int(data[13:16])
            pulses_ch4 = int(data[17:20])
            pulses_check = int(data[21:25])            
        except:
            print("bad pulses number: " + data[5:8])
            print("bad pulses number: " + data[9:12])
            print("bad pulses number: " + data[13:16])
            print("bad pulses number: " + data[17:20])
            print("bad pulses number: " + data[21:25])            
            return 0
        
        pkt.sequence = sequence
        pkt.pulses_ch1 = pulses_ch1
        pkt.pulses_ch2 = pulses_ch2
        pkt.pulses_ch3 = pulses_ch3
        pkt.pulses_ch4 = pulses_ch4
        pkt.pulses_check = pulses_check
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


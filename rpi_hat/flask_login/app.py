import eventlet
eventlet.monkey_patch()
from flask import Flask
from flask import Flask, flash, redirect, render_template, request, session, abort, Response, url_for, send_from_directory
from flask_socketio import SocketIO
import json
import os
from blinker import signal
import threading


### GLOBALS FOR CONFIGURATION #########
from get_distroname import GetDistroName
from flash_data import ReadConfigFile, WriteConfigFile, hourUpdate, getMeterTotals, getMeterHours, getMeterWeeks, getMeterMonths


## OS where its run
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

    
app = Flask(__name__)
app.secret_key = os.urandom(12)
socketio = SocketIO(app)

meas_signal = signal('meas_signal')
one_hour_signal = signal('one_hour_signal')

pulses_current_hour = [ 0, 0, 0, 0]

def getPulses (pulses_conf):
    global pulses_current_hour
    
    m1 = 0
    m2 = 0
    m3 = 0
    m4 = 0
    
    if button_last_conf == 'real':
        pulses_total = getMeterTotals()
        m1 = pulses_total[0] + pulses_current_hour[0]
        m2 = pulses_total[1] + pulses_current_hour[1]        
        m3 = pulses_total[2] + pulses_current_hour[2]
        m4 = pulses_total[3] + pulses_current_hour[3]
    elif button_last_conf == 'last_hour':
        m1 = pulses_current_hour[0]
        m2 = pulses_current_hour[1]
        m3 = pulses_current_hour[2]
        m4 = pulses_current_hour[3]
    elif button_last_conf == 'last_day':
        (last_day_m1, last_day_m2, last_day_m3, last_day_m4) = getMeterHours()
        m1 = sum(last_day_m1) + pulses_current_hour[0]
        m2 = sum(last_day_m2) + pulses_current_hour[1]
        m3 = sum(last_day_m3) + pulses_current_hour[2]
        m4 = sum(last_day_m4) + pulses_current_hour[3]
    elif button_last_conf == 'last_week':
        (last_day_m1, last_day_m2, last_day_m3, last_day_m4) = getMeterHours()
        (last_week_m1, last_week_m2, last_week_m3, last_week_m4) = getMeterWeeks()
        m1 = sum(last_day_m1) + sum(last_week_m1) + pulses_current_hour[0]
        m2 = sum(last_day_m2) + sum(last_week_m2) + pulses_current_hour[1]
        m3 = sum(last_day_m3) + sum(last_week_m3) + pulses_current_hour[2]
        m4 = sum(last_day_m4) + sum(last_week_m4) + pulses_current_hour[3]
    elif button_last_conf == 'last_month':
        (last_day_m1, last_day_m2, last_day_m3, last_day_m4) = getMeterHours()
        (last_month_m1, last_month_m2, last_month_m3, last_month_m4) = getMeterMonths()        
        m1 = sum(last_day_m1) + sum(last_month_m1) + pulses_current_hour[0]
        m2 = sum(last_day_m2) + sum(last_month_m2) + pulses_current_hour[1]
        m3 = sum(last_day_m3) + sum(last_month_m3) + pulses_current_hour[2]
        m4 = sum(last_day_m4) + sum(last_month_m4) + pulses_current_hour[3]

    return (m1, m2, m3, m4)


def setPulses (cntrs_list):
    global pulses_current_hour
    global button_last_conf

    m1 = 0
    m2 = 0
    m3 = 0
    m4 = 0
    
    pulses_current_hour[0] += cntrs_list[1]
    pulses_current_hour[1] += cntrs_list[2]
    pulses_current_hour[2] += cntrs_list[3]
    pulses_current_hour[3] += cntrs_list[4]

    (m1, m2, m3, m4) = getPulses(button_last_conf)

    if cntrs_list[1]:
        socketio.emit('meter1', {'data': str(m1)})

    if cntrs_list[2]:
        socketio.emit('meter2', {'data': str(m2)})

    if cntrs_list[3]:
        socketio.emit('meter3', {'data': str(m3)})

    if cntrs_list[4]:
        socketio.emit('meter4', {'data': str(m4)})
        
    
def sendAllMeters (to_sockets, button_sel='real'):
    (m1, m2, m3, m4) = getPulses (button_sel)
    to_sockets.emit('meter1', {'data': str(m1)})
    to_sockets.emit('meter2', {'data': str(m2)})
    to_sockets.emit('meter3', {'data': str(m3)})
    to_sockets.emit('meter4', {'data': str(m4)})

                    
@app.route('/')
def home():
    if not session.get('logged_in'):
        return redirect(url_for('do_admin_login'), code=302)
    else:
        return render_template('meter.html')

    
@app.route('/index.html')
def home_index():
    return redirect(url_for('do_admin_login'), code=302)


@app.route('/login', methods=['GET', 'POST'])
def do_admin_login():
    if request.method == 'GET':
        return render_template('index.html')

    if request.method == 'POST':
        if (request.form['password'] == 'password' and request.form['username'] == 'admin') or \
           (request.form['password'] == 'maxi' and request.form['username'] == 'maxi') or \
           (request.form['password'] == 'Paul' and request.form['username'] == 'Paul'):
            session['username'] = request.form['username']
            session['logged_in'] = True
            return redirect(url_for('home'), code=302)
        else:
            return redirect(url_for('no_login_page'), code=302)


@app.route('/no-login')
def no_login_page():
    return render_template('no-login.html')


@app.route('/favicon2.ico')
def favicon():
    return send_from_directory(os.path.join(app.root_path, 'templates'),
                               'favicon2.ico', mimetype='image/vnd.microsoft.icon')
""" 
    Socket-IO
    The names message, json, connect and disconnect are reserved and cannot be used for named events
"""

button_last_conf = 'real'

@socketio.on('connect')
def test_connect():
    if session.get('username'):        
        print('Client ' + str(session.get('username')) + ' Connected!')
        socketio.emit('button_in', {'data': button_last_conf})
        sendAllMeters(socketio, button_last_conf)
        LedConnect()
    else:
        print('Client not recognized')
        socketio.emit('redirect', {'data': str(session.get('username'))})
        

@socketio.on('disconnect')
def test_disconnect():
    print('Client disconnected')
    LedDisconnect()


@socketio.on('botones')
def handle_message(message):
    global button_last_conf
    
    print('received message: ' + str(message))

    # change type of meas here
    if message['data'] == 'real' or \
       message['data'] == 'last_hour' or \
       message['data'] == 'last_day' or \
       message['data'] == 'last_week' or \
       message['data'] == 'last_month':
        
        button_last_conf = message['data']
        socketio.emit('button_in', {'data': button_last_conf})
        sendAllMeters(socketio, button_last_conf)



###########
# Signals #
###########
meas_signal.connect(setPulses)
# one_hour_signal.connect(one_hour_timer_process)

    
#########################
# One Hour Update Timer #
#########################
def one_hour_timer_handler ():
    print("  one hour pass") 
    one_hour_signal.send()    
    one_hour_timer = threading.Timer(interval=3600, function=one_hour_timer_handler)
    # one_hour_timer = threading.Timer(interval=60, function=one_hour_timer_handler)    
    one_hour_timer.start()

ReadConfigFile()    # start with saved values
# ReadConfigFile(True)    # start with empty values
uptime = 0
one_hour_timer = threading.Timer(interval=3600, function=one_hour_timer_handler)
# one_hour_timer = threading.Timer(interval=60, function=one_hour_timer_handler)
one_hour_timer.start()

def one_hour_timer_process (mymy):
    global pulses_current_hour
    global uptime

    uptime += 1
    print("  one hour signal process, uptime: " + str(uptime) + ' hours')
    hourUpdate(pulses_current_hour)

    pulses_current_hour[0] = 0
    pulses_current_hour[1] = 0
    pulses_current_hour[2] = 0
    pulses_current_hour[3] = 0

one_hour_signal.connect(one_hour_timer_process)

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
    print("  in signals!!! meas list: " + str(meas_list))
    meas_signal.send(meas_list)


myseq = 0
mytest_state = 'init'
def bkg_test ():
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
        
        
    # mtt = threading.Timer(interval=0.5, function=myTestTimeout)
    mtt = threading.Timer(interval=4, function=bkg_test)
    mtt.start()


def LedDisconnect():
    global my_proc

    my_proc.LedServer('toggle off')

def LedConnect():
    global my_proc

    my_proc.LedServer('toggle on')    

    
########
# Main #
########
if __name__ == "__main__":
    bkg_process()
    # app.secret_key = os.urandom(12)
    # app.run(debug=True,host='0.0.0.0', port=5000)    #    app.run(host='0.0.0.0', debug=True, threaded=True)
    # socketio.run(app, host='0.0.0.0', debug=True)
    socketio.run(app, host='0.0.0.0', debug=True, use_reloader=False)    #werkzeug no reload


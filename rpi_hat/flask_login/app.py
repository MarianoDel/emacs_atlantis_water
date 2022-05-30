import eventlet
eventlet.monkey_patch()
from flask import Flask
from flask import Flask, flash, redirect, render_template, request, session, abort, Response, url_for, send_from_directory
from flask_socketio import SocketIO
import json
import pyaudio
import os
from blinker import Namespace

import threading
import numpy as np

### GLOBALS FOR CONFIGURATION #########
from get_distroname import GetDistroName

## OS where its run
distname = GetDistroName()
if distname == 'debian':
    RUNNING_ON_SLACKWARE = 0
    RUNNING_ON_RASP = 1
elif distname == 'Slackware ':
    RUNNING_ON_SLACKWARE = 1
    RUNNING_ON_RASP = 0
else:
    print('No distro finded!')
    exit(-1)

app = Flask(__name__)
app.secret_key = os.urandom(12)
socketio = SocketIO(app)

my_signal = Namespace()
audio_signal = my_signal.signal('audio signal')

## init of gpios and steady state
if RUNNING_ON_RASP:
    from gpios import *
    GpiosInit()
    PttOff()
    LedBlueOff()
    OnOff_On()
    Channel_to_Memory('12')


pulses_total = [10000, 20000, 30000, 40000]
pulses_d30 = [1000, 2000, 3000, 4000]
pulses_d7 = [100, 200, 300, 400]
pulses_d1 = [10, 20, 30, 40]
pulses_h1 = [1, 2, 3, 4]

def getPulses (pulses_conf):
    m1 = 0
    m2 = 0
    m3 = 0
    m4 = 0
    
    if button_last_conf == 'real':
        m1 = pulses_total[0]
        m2 = pulses_total[1]
        m3 = pulses_total[2]
        m4 = pulses_total[3]
    elif button_last_conf == 'last_hour':
        m1 = pulses_h1[0]
        m2 = pulses_h1[1]
        m3 = pulses_h1[2]
        m4 = pulses_h1[3]
    elif button_last_conf == 'last_day':
        m1 = pulses_d1[0]
        m2 = pulses_d1[1]
        m3 = pulses_d1[2]
        m4 = pulses_d1[3]
    elif button_last_conf == 'last_week':
        m1 = pulses_d7[0]
        m2 = pulses_d7[1]
        m3 = pulses_d7[2]
        m4 = pulses_d7[3]
    elif button_last_conf == 'last_month':
        m1 = pulses_d30[0]
        m2 = pulses_d30[1]
        m3 = pulses_d30[2]
        m4 = pulses_d30[3]

    return (m1, m2, m3, m4)

        
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
           (request.form['password'] == 'maxi' and request.form['username'] == 'maxi'):
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
    else:
        print('Client not recognized')
        socketio.emit('redirect', {'data': str(session.get('username'))})
        

@socketio.on('disconnect')
def test_disconnect():
    print('Client disconnected')
    # audio_stop_stream()    #no encuentra el thread
    if RUNNING_ON_RASP:
        LedBlueToggleContinous('stop')


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


audio_data_rx = 0
def send_custom(message):
    print("custom msg" + str(message))
    socketio.emit('audio_start', 'in signal')

audio_signal.connect(send_custom)

################################
# Rutinas de Audio lado Server #
################################
SOUND_HARCODED = 0
SOUND_MIC_INPUT = 1

if SOUND_HARCODED:    
    freq = 440
    samplerate = 44100
    timeloop = 0.5
    amplitude = 1.0
    frames_qtty = int(samplerate * timeloop)
    # generate audio data
    data = np.zeros(frames_qtty, dtype=np.float32)
    for i in range(frames_qtty):
        data[i] = np.sin(np.pi * 2 * freq * i / samplerate) * amplitude

    data_bytes = data.tobytes()

    harcoded_audio_thread = threading.Thread()
    pckt_cnt = 0
    

if SOUND_MIC_INPUT:
    FORMAT = pyaudio.paInt16
    # FORMAT = pyaudio.paFloat32
    # FORMAT = pyaudio.paInt32
    CHANNELS = 1
    RATE = 44100
    TIME_CHUNK = 0.5
    # TIME_CHUNK = 1    
    CHUNK = int(RATE * TIME_CHUNK)

    audio_dev = pyaudio.PyAudio()
    audio_stream = audio_dev


playing = False


@socketio.on('audio')
def play_or_pause(message):
    global playing
    global audio_stream
    
    if message['data'] == 'PLAY':
        print("empezar audio por sockets")
        socketio.emit('audio_start', 'before callback')
        if playing != True:
            playing = True
            print("recording...")

            #######################
            # Envia harcoded sine #
            #######################
            if SOUND_HARCODED:
                audio_generation_start()

            ##############################################
            # Envia lo conseguido en el MIC por callback #
            ##############################################
            if SOUND_MIC_INPUT:
                # audio_stream = audio_dev.open(format=FORMAT,
                #                               channels=CHANNELS,
                #                               rate=RATE,
                #                               frames_per_buffer=CHUNK,
                #                               output=True,
                #                               input=True,
                #                               stream_callback=audio_mic_input_callback)

                audio_stream = audio_dev.open(format=FORMAT,
                                              channels=CHANNELS,
                                              rate=RATE,
                                              frames_per_buffer=CHUNK,
                                              input=True,
                                              stream_callback=audio_mic_input_callback)
                
                audio_stream.start_stream()

    elif message['data'] == 'STOP':
        #######################################
        # Solo si es por MIC reviso el stream #
        #######################################
        if SOUND_MIC_INPUT:
            audio_stop_stream()

        print("terminar audio")
        playing = False


def audio_stop_stream ():
    global audio_stream
    
    if audio_stream.is_active():
        audio_stream.stop_stream()
        audio_stream.close()


def audio_mic_input_callback(in_data, frame_count, time_info, status):
    global audio_data_rx
    # if FORMAT == pyaudio.paInt16:
    #     socketio.emit('audio_start', 'after callbak')
    #     # socketio.emit('audio_int16', {'data': in_data})
    # elif FORMAT == pyaudio.paInt32:
    #     socketio.emit('audio_int32', {'data': in_data})
    # else:
    #     socketio.emit('audio_f32', {'data': in_data})

    # para pruebas de callbak
    # socketio.emit('audio_int16', {'data': in_data})
    socketio.emit('audio_start', 'after callbak')
    print('.')
    audio_signal.send()

    # fin para pruebas callbak
    
    if status != 0:
        if status == pyaudio.paInputUnderflow:
            print("Buffer underflow in input")
        elif status == pyaudio.paInputOverflow:
            print("Buffer overflow in input")
        elif status == pyaudio.paOutputUnderflow:
            print("Buffer underflow in output")
        elif status == pyaudio.paOutputOverflow:
            print("Buffer overflow in output")
        elif status == pyaudio.paPrimingOutput:
            print("Just priming, not playing yet")

    audio_data_rx = in_data
    data_to_play = in_data    #sale lo mismo que entro en el mic
    return (data_to_play, pyaudio.paContinue)


def audio_generation_start():
    global harcoded_audio_thread
    global pckt_cnt    
    # Create your thread
    harcoded_audio_thread = threading.Timer(0.05, audio_generation_callback, ())
    harcoded_audio_thread.start()
    pckt_cnt = 0


def audio_generation_callback():
    global playing
    global harcoded_audio_thread
    global pckt_cnt
    global data_bytes
    global timeloop
    
    if playing == True:
        # print (data)
        # socketio.emit('audio_rx', {'data': data_bytes})
        socketio.emit('audio_f32', {'data': data_bytes})
        # call next loop
        harcoded_audio_thread = threading.Timer(timeloop, audio_generation_callback, ())
        harcoded_audio_thread.start()
        pckt_cnt = pckt_cnt + 1
        print('gen: ' + str(pckt_cnt))


if __name__ == "__main__":
    # app.secret_key = os.urandom(12)
    # app.run(debug=True,host='0.0.0.0', port=5000)    #    app.run(host='0.0.0.0', debug=True, threaded=True)
    socketio.run(app, host='0.0.0.0', debug=True)


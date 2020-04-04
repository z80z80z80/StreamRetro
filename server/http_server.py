#!/usr/bin/env python3

import socket
import zmq
import sys, os
import logging
import time
import base64
import threading

from flask import Flask, request, render_template, redirect
from flask_socketio import SocketIO, emit

app = Flask(__name__)

app.config['SECRET_KEY'] = 'StreamRetro'
socketio = SocketIO(app)

app.debug = False

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
log.disabled = True

app.logger.disabled = True

# Create a ZeroMQ socket (as Publisher)
context = zmq.Context.instance(1)
publisher = context.socket(zmq.PUB)
publisher.bind("tcp://127.0.0.1:1312")

key = 0
key_old = 0

global thread
global running 
running = 0
thread = 0
stop_event= threading.Event()

def send(key):
    # Send data    
    publisher.send_string(key)    
    publisher.send_string(key)    

def launch(system, rom, stop_event):
    os.system("../../streamRetro /usr/lib/libretro/%s ../roms/%s > /dev/null &" % (system, rom))
    
def run(system, rom):
    global thread
    if thread:
        stop_event.set()   
    thread = threading.Thread(target=os.system, args=["./../bin/streamRetro /usr/lib/libretro/%s ../roms/%s > /dev/null" % (system, rom)], daemon=True).start()

@app.route("/")#, methods=['GET', 'POST'])
def index():    
    global running
    if not running:
        threading.Thread(target=os.system, args=["python zmq/relais.py"], daemon=True).start()                   
        running = 1
    return render_template(template)

@socketio.on('connect', namespace='/retro')
def test_connect():
    emit('my_response', {'data': 'Connected'})
    print('my_response', {'data': 'Connected'})

@socketio.on('event', namespace='/retro')
def test_message(message):
    key = message['data']
    send(key)
    #print(key)
    '''    
    emit('my_response', {'data': message['data']}, broadcast=True)
    if request.method == "POST":
        key = request.form["key"]
        app.logger.error(key)
        send(key)     
        send(key)    
        key = 0
    '''
'''
@socketio.on('change_game', namespace='/retro')
def test_screen(message):
    screen_sz = run(message.system, message.rom)
    emit('screen', screen_sz, broadcast=True)
'''   

@socketio.on('data_in', namespace='/retro')
def test_data(message):
    #img = base64.b64decode(message.encode())
    emit('data_out', message, broadcast=True)
    #print(message[-10:])
    #f = open("static/img.jpg", "wb")
    #f.write(img)
    #f.close()
    
if __name__=="__main__":
    channel = 1
    ap = sys.argv[1]
    threading.Thread(target=os.system, args=["create_ap -n -c %s --redirect-to-localhost -w 1+2 %s MarikoDoom > /dev/null" % (channel, ap)], daemon=True).start()                   

    run("gambatte_libretro.so", "gbai.gb")

    template = "index_20fps.html"
    socketio.run(app, host="0.0.0.0", port=80)

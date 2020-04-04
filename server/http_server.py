#!/usr/bin/env python3

import socket
import zmq
import sys
import logging
import time
import base64

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

def send(key):
    # Send data    
    publisher.send_string(key)    

@app.route("/")#, methods=['GET', 'POST'])
def index():    
    return render_template(template)

@socketio.on('connect', namespace='/retro')
def test_connect():
    emit('my_response', {'data': 'Connected'})
    print('my_response', {'data': 'Connected'})

@socketio.on('event', namespace='/retro')
def test_message(message):
    key = message['data']
    send(key)
    send(key)
    print(key)
    '''    
    emit('my_response', {'data': message['data']}, broadcast=True)
    if request.method == "POST":
        key = request.form["key"]
        app.logger.error(key)
        send(key)     
        send(key)    
        key = 0
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
    port = sys.argv[1]    
    fps = int(sys.argv[2])

    if fps == 1:
        template = "index_15fps.html"
    else:
        template = "index_20fps.html"
    socketio.run(app, host="0.0.0.0", port=port)

#!/usr/bin/env python3

import socket
import zmq
import sys
import logging
import time

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

@app.route("/", methods=['GET', 'POST'])
def index():    
    if request.method == "POST":
        key = request.form["key"]
        app.logger.error(key)
        send(key)     
        send(key)    
        key = 0

    return render_template(template)

if __name__=="__main__":
    port = sys.argv[1]    
    fps = int(sys.argv[2])

    if fps == 1:
        template = "index_15fps.html"
    else:
        template = "index_20fps.html"
    socketio.run(app, host="0.0.0.0", port=port)

import zmq
import socketio
import time
import base64

# Create a ZeroMQ socket (as subscriber): C++ -> relais.py (this)
context_sub = zmq.Context.instance(1)
subscriber = context_sub.socket(zmq.SUB)
subscriber.connect("tcp://127.0.0.1:4242")
subscriber.setsockopt_string(zmq.SUBSCRIBE, "")

# Create a socket.io websocket: relais.py (this) ->  Switch
sio = socketio.Client()
sio.connect('http://localhost:80', namespaces=["/retro"])

rec = 0
rec_old = 0
i = 0

frameskip = 2 # 30fps until we can stream directly to the browser from either this relais or Core.cpp
while True:
    i += 1
    rec = subscriber.recv()     
    '''
    if i%30 == 0:
        print(type(rec))
    '''
    if i%frameskip == 0:
        sio.emit('data_in', base64.b64encode(rec).decode(), '/retro')
        #sio.emit('data_in', {'data': "test"}, namespace='/retro')
        print(type(base64.b64encode(rec).decode()))
    rec_old = rec
    rec = 0     

subscriber.close()
context_sub.destroy()

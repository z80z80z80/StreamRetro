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

frameskip = 3 # 20-30fps seems to be all the Switch can handle (keeping it at 20fps for now)
while True:
    i += 1
    rec = subscriber.recv()     
    '''
    if i%30 == 0:
        print(type(rec))
    '''
    if i%frameskip == 0:
        try:
            sio.emit('data_in', rec, '/retro')
        except:
            print("too many packages in the loop")
        #sio.emit('data_in', base64.b64encode(rec).decode(), '/retro')
        #sio.emit('data_in', {'data': "test"}, namespace='/retro')
    if i%60 == 0:        
        print(type(rec))
    rec_old = rec
    rec = 0     

subscriber.close()
context_sub.destroy()

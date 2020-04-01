import zmq

context = zmq.Context.instance()
subscriber = context.socket(zmq.SUB)
subscriber.connect("tcp://localhost:5556")
subscriber.setsockopt_string(zmq.SUBSCRIBE, "")

rec = 0
while True:
    try:
        rec = subscriber.recv_string(zmq.NOBLOCK)     
    except zmq.error.Again:
        print("nothing")
    if rec:
        print(rec)
        rec = 0

subscriber.close()
context.destroy()

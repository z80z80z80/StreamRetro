import zmq

context_sub = zmq.Context.instance(1)
subscriber = context_sub.socket(zmq.SUB)
subscriber.connect("tcp://127.0.0.1:1312")
subscriber.setsockopt_string(zmq.SUBSCRIBE, "")

context_pub = zmq.Context.instance(1)
publisher = context_pub.socket(zmq.PUB)
publisher.bind("tcp://127.0.0.1:4242")

rec = 0
rec_old = 0
while True:
    try:
        rec = subscriber.recv_string(zmq.NOBLOCK)     
    except zmq.error.Again:
        if rec_old:
            publisher.send_string(rec_old)
            rec_old = 0
    if rec:
        print(rec)
        publisher.send_string(rec)
        rec_old = rec
        rec = 0     

subscriber.close()
publisher.close()
context_s.destroy()
context_p.destroy()

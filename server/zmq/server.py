import zmq

context = zmq.Context.instance(1)
publisher = context.socket(zmq.PUB)
publisher.bind("tcp://127.0.0.1:1312")

while True:
    publisher.send_string(str(type("hello")))

publisher.close()
context.destroy()

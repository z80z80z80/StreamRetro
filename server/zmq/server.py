import zmq

context = zmq.Context.instance(1)
publisher = context.socket(zmq.PUB)
publisher.bind("tcp://*:5556")

while True:
    publisher.send_string("hello")

publisher.close()
context.destroy()

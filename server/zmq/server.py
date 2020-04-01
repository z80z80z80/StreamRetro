import zmq
from time import sleep

context = zmq.Context.instance()
publisher = context.socket(zmq.PUB)
publisher.bind("tcp://*:5556")

for _ in range(100):
    publisher.send_string("Hello")
    sleep(1)

publisher.close()
context.destroy()

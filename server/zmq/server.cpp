//  g++ -lzmq server.cpp -o server.o -w

#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>

int main () {

    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");

    while (1) {
        zmq::message_t message(6);
        memcpy((char*) message.data(), "hello", 6);

        publisher.send(message);

    }
    return 0;
}


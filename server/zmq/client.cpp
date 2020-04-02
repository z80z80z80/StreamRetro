//  g++ -lzmq client.cpp -o client.o -w

#include <zmq.hpp>
#include "zhelpers.hpp"
#include <iostream>
#include <sstream>

int main (int argc, char *argv[])
{
    zmq::context_t context (1);

    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");

    const char *filter = "h";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    std::string message;

    while (true) {        

        message = s_recv(subscriber, ZMQ_DONTWAIT);

        if (errno != EAGAIN){          
            std::cout << message + "\n";           
        }
        else{
            std::cout << "nothing\n";
            errno = 0;
        }

    }
    return 0;
}


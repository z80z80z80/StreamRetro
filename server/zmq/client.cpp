//  g++ -lzmq client.cpp -o client.o -w

#include <zmq.hpp>
#include "zhelpers.hpp"
#include <iostream>
#include <unistd.h>

int main ()
{
    zmq::context_t context (1);

    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://127.0.0.1:4242");

    const char *filter = "";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    std::string message;

    while (true) {        

        message = s_recv(subscriber, ZMQ_NOBLOCK);

        if (errno != EAGAIN){          
            std::cout << message + "\n";           
        }
        else{
            //std::cout << "nothing\n";
            errno = 0;
        }
        //usleep(10000);

    }
    return 0;
}


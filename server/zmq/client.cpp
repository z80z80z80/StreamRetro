//  g++ -lzmq client.cpp -o client.o -w

#include <zmq.hpp>
#include <iostream>
#include <sstream>

int main (int argc, char *argv[])
{
    zmq::context_t context (1);

    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");

    const char *filter = "h";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    while (true) {

        zmq::message_t message;

        subscriber.recv(&message, ZMQ_DONTWAIT);

        if (errno != EAGAIN){
            std::string str = static_cast<char*>(message.data());
            std::cout << str + "\n";           
        }
        else{
            std::cout << "nothing\n";
            errno = 0;
        }

    }
    return 0;
}


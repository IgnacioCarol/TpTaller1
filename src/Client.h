#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H


#include <string>
#include "Socket.h"

class Client {
public:
    Client(const char * IP, const char * port);
    int init();
    bool isConnected();
    bool send(const void* msg, size_t len);
    bool receive(const void* msg, size_t len);
    void release();
private:
    const char * _IP;
    const char * _port;
    Socket* _socket;

};


#endif //TPTALLER1_CLIENT_H

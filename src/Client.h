#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H


#include <string>
#include "Socket/Socket.h"

class Client {
public:
    Client(std::string IP, std::string port);
    int init();
    bool isConnected();
    int send(msg_t *msg);
    bool receive(msg_t *msg, size_t len);
    void release();
private:
    const char * _IP;
    const char * _port;
    Socket* _socket;

};


#endif //TPTALLER1_CLIENT_H

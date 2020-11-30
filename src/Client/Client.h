#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H


#include <string>
#include "../Socket/Socket.h"
#include "../logger/logger.h"
#include "ClientMsg.h"
#include <exception>

class Client {
public:
    Client(std::string IP, std::string port);
    ~Client();
    void init();
    bool isConnected();
    int send(msg_t *msg, size_t len);
    int receive(msg_t *msg, size_t len);
    void release();

private:
    const char * _IP;
    const char * _port;
    Socket* _socket;
    std::string _clientID; //TODO inicializar con el XML? Tal vez el ID pueda ser el nombre de usuario, una vez que se haga el login utilizar username en este campo

};


#endif //TPTALLER1_CLIENT_H

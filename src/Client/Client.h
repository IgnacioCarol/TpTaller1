#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H


#include <string>
#include "src/Socket/Socket.h"

class Client {
public:
    Client(std::string IP, std::string port);
    ~Client();
    bool init();
    bool isConnected();
    bool send(msg_t *msg, size_t len);
    bool receive(msg_t *msg, size_t len);
    void release();
private:
    const char * _IP;
    const char * _port;
    Socket* _socket;
    const char * _clientID; //TODO inicializar con el XML? Tal vez el ID pueda ser el nombre de usuario

};


#endif //TPTALLER1_CLIENT_H

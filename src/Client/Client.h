#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H

#include <string>
#include <exception>
#include "json.hpp"
#include "../Socket/Socket.h"
#include "../logger/logger.h"
#include "../Login/Login.h"
#include "../Game.h"
#include "ClientMsg.h"

using json = nlohmann::json;

class Client {
public:
    Client(std::string IP, std::string port);
    ~Client();
    void init();
    void play();
    bool isConnected();
    int send(json *msg);
    int receive(json *msg);
    void release();

private:
    const char * _IP;
    const char * _port;
    Socket* _socket;
    std::string _clientID; //TODO inicializar con el XML? Tal vez el ID pueda ser el nombre de usuario, una vez que se haga el login utilizar username en este campo
    Login* login;

    bool authenticate();
};


#endif //TPTALLER1_CLIENT_H

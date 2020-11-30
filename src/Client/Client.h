#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H

#include <string>
#include "json.hpp"
#include "../Socket/Socket.h"
using json = nlohmann::json;

class Client {
public:
    Client(std::string IP, std::string port);
    ~Client();
    bool init();
    bool isConnected();
    int send(json *msg);
    bool receive(json *msg);
    void release();
private:
    const char * _IP;
    const char * _port;
    Socket* _socket;
    std::string _clientID; //TODO inicializar con el XML? Tal vez el ID pueda ser el nombre de usuario, una vez que se haga el login utilizar username en este campo

};


#endif //TPTALLER1_CLIENT_H

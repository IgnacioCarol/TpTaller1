#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H


#include <string>
#include "Socket/Socket.h"
using json = nlohmann::json;
class Client {
public:
    Client(std::string IP, std::string port);
    ~Client();
    int init();
    bool isConnected();
    int send(json *msg);
    bool receive(json *msg);
    void release();
private:
    const char * _IP;
    const char * _port;
    Socket* _socket;

};


#endif //TPTALLER1_CLIENT_H

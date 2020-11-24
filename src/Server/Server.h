//
// Created by Daniel Bizari on 23/11/2020.
//

#ifndef TPTALLER1_SERVER_H
#define TPTALLER1_SERVER_H

#include <sstream>
#include <iostream>
#include "../Socket/SocketException.h"
#include "../Socket/Socket.h"
#include "../logger/logger.h"

class Server {

public:
    static Server * getInstance();
    virtual ~Server();

    bool init(const char *ip, const char *port, int clientNo);
    //bool run();

private:
    static Server * instance;
    Server();

    bool initSocket(const char*ip, const char *port);
    bool acceptClients(int clientNo);
    bool receive(Socket *client);

    Socket *_socket;
    std::vector<Socket *> clients;

    const int MAX_ACCEPT_RETRIES = 10;
};


#endif //TPTALLER1_SERVER_H

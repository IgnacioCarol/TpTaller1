//
// Created by Daniel Bizari on 23/11/2020.
//

#include <sstream>
#include "Server.h"
#include "../Socket/SocketException.h"
#include "../Socket/Socket.h"
#include "../logger/logger.h"

Server* Server::instance = nullptr;

Server *Server::getInstance() {
    if (Server::instance == nullptr) {
        Server::instance = new Server();
    }

    return instance;
}

Server::~Server() {

}

Server::Server() {

}

bool Server::run() {
    try {
        auto * serverSocket = new Socket();
        serverSocket->init("127.0.0.1", "8080", SERVER);
        serverSocket->bindAndListen();
        std::cout << "server is up and running" << std::endl;
        Socket * client = serverSocket->accept();
        //ToDo por cada nuevo cliente que se acepta se debe crear un thread
        msg_t message;
        memset(&message, 0, sizeof(message));
        client->receive(&message, sizeof(message));

        std::stringstream ss;
        ss << "val1: " << message.val1 << std::endl
           << "val2: " << message.val2 << std::endl
           << "val3: " << message.val3 << std::endl
           << "val4: " << message.val4 << std::endl
           << "val5: " << message.val5 << std::endl
           << "val6: " << message.val6 << std::endl
           << "val7: " << message.val7 << std::endl;
        Logger::getInstance()->info(ss.str());
    } catch (std::exception &ex) {
        //ToDo handle error
        Logger::getInstance()->error(ex.what());
        return false;
    }

    return true;
}



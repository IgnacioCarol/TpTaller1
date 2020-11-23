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
        client->receive(&message, sizeof(message));

        std::stringstream ss;
        ss << "value: " << message.value << " text: " << message.text;
        Logger::getInstance()->info(ss.str());
    } catch (std::exception &ex) {
        //ToDo handle error
        Logger::getInstance()->error(ex.what());
        return false;
    }

    return true;
}



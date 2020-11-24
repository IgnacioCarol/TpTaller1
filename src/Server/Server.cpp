//
// Created by Daniel Bizari on 23/11/2020.
//

#include "Server.h"

Server* Server::instance = nullptr;

Server *Server::getInstance() {
    if (Server::instance == nullptr) {
        Server::instance = new Server();
    }

    return instance;
}

Server::~Server() {
    Logger::getInstance()->info("Destroying server");
    delete _socket;
    for (int i = 0; i < clients.size(); i++) {
        delete clients[i];
    }
}

Server::Server() {

}

bool Server::init(const char *ip, const char *port, int clientNo) {
    if (!initSocket(ip, port)) {
        return false;
    }
    Logger::getInstance()->info("Server is up and running");

    if (!acceptClients(clientNo)) {
        return false;
    }
    Logger::getInstance()->info("All clients have been accepted");

    //TODO: Multithreading
    if(!receive(clients[0])) {
        return false;
    }
    Logger::getInstance()->info("Messages received");
    return true;
}

bool Server::initSocket(const char*ip, const char *port) {
    _socket = new Socket();
    _socket->init(ip, port, SERVER);
    return _socket->bindAndListen();
}

bool Server::acceptClients(int clientNo) {
     int retry = 1;

    for (int i = 0; i < clientNo && retry <= MAX_ACCEPT_RETRIES; i++, retry++) {
        try {
            clients.push_back(_socket->accept());
            Logger::getInstance()->info("Client number " + std::to_string(i) + " has been accepted");
        } catch (std::exception &ex) {
            Logger::getInstance()->error("Error accepting client number: " + std::to_string(i));
            i--;
        }
    }

    if (retry == MAX_ACCEPT_RETRIES) {
        return false;
    }
    return true;
}

bool Server::receive(Socket *client) {
    msg_t message;
    memset(&message, 0, sizeof(message));

    if (client->receive(&message, sizeof(message)) == 0) {
        Logger::getInstance()->error("Couldnt receive message from client"); //TODO: se puede mejorar el log identificando el cliente
        return false;
    }

    //TODO: Borrar, es solo para prueba
    std::stringstream ss;
    ss << "val1: " << message.val1 << std::endl
       << "val2: " << message.val2 << std::endl
       << "val3: " << message.val3 << std::endl
       << "val4: " << message.val4 << std::endl
       << "val5: " << message.val5 << std::endl
       << "val6: " << message.val6 << std::endl
       << "val7: " << message.val7 << std::endl;
    Logger::getInstance()->info(ss.str());
    return true;
}

/*
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
*/


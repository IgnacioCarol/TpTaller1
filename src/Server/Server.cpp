//
// Created by Daniel Bizari on 23/11/2020.
//

#include "Server.h"
#include <pthread.h>

Server* Server::instance = nullptr;

Server *Server::getInstance() {
    if (Server::instance == nullptr) {
        Server::instance = new Server();
    }

    return instance;
}

Server::~Server() {
    Logger::getInstance()->info("[Server] Destroying server");
    delete _socket;
    for (auto & client : clients) {
        delete client;
    }
    pthread_mutex_destroy(this->commandMutex);
    free(this->threads);
}

Server::Server() {
    pthread_mutex_init(this->commandMutex, nullptr);
}

bool Server::init(const char *ip, const char *port, int clientNo) {
    // Init threads
    this->clientNo = clientNo;
    this->threads = (pthread_t *) (malloc(sizeof(pthread_t) * clientNo));
    if (!this->threads) {
        Logger::getInstance()->error("[Server] unable to create threads array, out of memory");
        return false;
    }

    if (!initSocket(ip, port)) {
        return false;
    }
    Logger::getInstance()->info("[Server] Server is up and running");

    if (!acceptClients()) {
        return false;
    }
    Logger::getInstance()->info("[Server] All clients have been accepted");

    //TODO: Multithreading
    if(!receive(clients[0]->getSocket())) { //ToDo me parece que queda mejor si el receive es un metodo del client
        return false;
    }
    Logger::getInstance()->info("[Server] Message received");
    return true;
}

bool Server::initSocket(const char*ip, const char *port) {
    _socket = new Socket();
    _socket->init(ip, port, SERVER);
    return _socket->bindAndListen();
}

bool Server::acceptClients() {
     int retry = 1;

    for (int i = 0; i < clientNo && retry <= MAX_ACCEPT_RETRIES; i++, retry++) {
        try {
            auto * playerClient = new PlayerClient(_socket->accept(), this->commandMutex);
            clients.push_back(playerClient);
            pthread_t *clientThread = nullptr;
            pthread_create(clientThread, nullptr, Server::handlePlayerClient, nullptr);
            Logger::getInstance()->info("[Server] Client number " + std::to_string(i) + " has been accepted");
        } catch (std::exception &ex) {
            Logger::getInstance()->error("[Server] Error accepting client number: " + std::to_string(i));
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
        Logger::getInstance()->error("[Server] Couldn't receive message from client"); //TODO: se puede mejorar el log identificando el cliente
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

void * Server::handlePlayerClient(void * arg) {

}

// Infinite loop processing PlayerClients commands
bool Server::run() {
    return false;
}


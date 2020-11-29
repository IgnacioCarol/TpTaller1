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
    pthread_mutex_destroy(&this->commandMutex);
    free(this->incomeThreads);
    free(this->outcomeThreads);
}

Server::Server() {
    pthread_mutex_init(&this->commandMutex, nullptr);
}

bool Server::init(const char *ip, const char *port, int clientNo) {
    // Init threads
    this->clientNo = clientNo;
    this->incomeThreads = (pthread_t *) (malloc(sizeof(pthread_t) * clientNo));
    if (!this->incomeThreads) {
        Logger::getInstance()->error("[Server] unable to create threads array, out of memory");
        return false;
    }

    this->outcomeThreads = (pthread_t *) (malloc(sizeof(pthread_t) * clientNo));
    if (!this->outcomeThreads) {
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
            auto * playerClient = new PlayerClient(_socket->accept(), &this->commandMutex, &this->commands);
            clients.push_back(playerClient);
            pthread_create(&incomeThreads[i], nullptr, Server::handlePlayerClient, (void *) playerClient);
            pthread_create(&outcomeThreads[i], nullptr, Server::broadcastToPlayerClient, (void *) playerClient);
            Logger::getInstance()->info("[Server] Client number " + std::to_string(i) + " has been accepted");
        } catch (std::exception &ex) {
            Logger::getInstance()->error("[Server] Error accepting client number: " + std::to_string(i));
            i--;
        }
    }

    return retry != MAX_ACCEPT_RETRIES;
}

void * Server::handlePlayerClient(void * arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    pthread_mutex_t  * mutex = playerClient->getCommandMutex();
    json msg;

    //ToDo while (playerClient->isConnected()) {
    while (true) {
        msg = playerClient->receive(); //ToDo realizar chequeo de si realmente el usuario mando algo y lo recibi, si no mando nada no deberia pushear a la cola de novedades

        pthread_mutex_lock(mutex);
        playerClient->commandQueue->push(msg);
        pthread_mutex_unlock(mutex);
    }

    return nullptr;
}

void * Server::broadcastToPlayerClient(void *arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    std::queue<json> * outcomeQueue = &playerClient->outcome;
    pthread_mutex_t  * mutex = playerClient->getOutcomeMutex();
    json msg;
    //ToDo while (playerClient->isConnected()) {
    while (true) {
        memset(&msg, 0, sizeof(msg));
        pthread_mutex_lock(mutex);
        if (!outcomeQueue->empty()) {
            msg = outcomeQueue->front();
            outcomeQueue->pop();
        } else {
            pthread_mutex_unlock(mutex);
            continue;
        }
        pthread_mutex_unlock(mutex);

        Logger::getInstance()->debug("envianding mensaje...");
        if(!playerClient->send(&msg)) {
            //ToDo handle error
            Logger::getInstance()->error("[SERVER] Error broadcasting message to client");
        }
    }
    return nullptr;
}

// Infinite loop processing PlayerClients commands
bool Server::run() {

    //ToDo while (Game->isRunning()) {
    while (true) {
        pthread_mutex_t  * mutex = &this->commandMutex;
        json message;
        memset(&message, 0, sizeof(message));

        pthread_mutex_lock(mutex);
        if (commands.size() > 0) {
            message = commands.front();
            commands.pop();
        }
        pthread_mutex_unlock(mutex);

        //ToDo change game state with msg

        if (!message.is_structured()) {
            continue;
        }

        std::stringstream ss;
        ss << "val1: " << message << std::endl;
        Logger::getInstance()->info(ss.str());

        for (auto & client : clients) {
            pthread_mutex_t * mutex = client->getOutcomeMutex();
            pthread_mutex_lock(mutex);
            client->outcome.push(message);
            pthread_mutex_unlock(mutex);
        }
    }

    return true;
}


#include "Server.h"
#include "ServerMsg.h"
#include <pthread.h>

Server* Server::instance = nullptr;
pthread_mutex_t logMutex;

Server *Server::getInstance() {
    if (Server::instance == nullptr) {
        Server::instance = new Server();
    }

    return instance;
}

Server::~Server() {
    pthread_mutex_lock(&logMutex);
    Logger::getInstance()->info(MSG_DESTROY_SERVER);
    pthread_mutex_unlock(&logMutex);
    delete _socket;
    for (auto & client : clients) {
        delete client;
    }
    pthread_mutex_destroy(&this->commandMutex);
    pthread_mutex_destroy(&logMutex);
    free(this->incomeThreads);
    free(this->outcomeThreads);
}

Server::Server() {
    pthread_mutex_init(&this->commandMutex, nullptr);
    pthread_mutex_init(&logMutex, nullptr);
}

void Server::init(const char *ip, const char *port, int clientNo) {
    // Init threads
    this->clientNo = clientNo;
    this->incomeThreads = (pthread_t *) (malloc(sizeof(pthread_t) * clientNo));
    if (!this->incomeThreads) {
        pthread_mutex_lock(&logMutex);
        Logger::getInstance()->error(MSG_NO_MEMORY_THREADS);
        pthread_mutex_unlock(&logMutex);
        throw ServerException(MSG_NO_MEMORY_THREADS);
    }

    this->outcomeThreads = (pthread_t *) (malloc(sizeof(pthread_t) * clientNo));
    if (!this->outcomeThreads) {
        pthread_mutex_lock(&logMutex);
        Logger::getInstance()->error(MSG_NO_MEMORY_THREADS);
        pthread_mutex_unlock(&logMutex);
        throw ServerException(MSG_NO_MEMORY_THREADS);;
    }

    initSocket(ip, port);
    pthread_mutex_lock(&logMutex);
    Logger::getInstance()->info(MSG_READY_SERVER);
    pthread_mutex_unlock(&logMutex);
    std::cout << MSG_READY_SERVER << std::endl; //TODO: Borrar
    acceptClients();
    pthread_mutex_lock(&logMutex);
    Logger::getInstance()->info(MSG_ALL_CLIENTS_ACCEPTED_SERVER);
    pthread_mutex_unlock(&logMutex);
}

void Server::initSocket(const char*ip, const char *port) {
    try {
        _socket = new Socket();
        _socket->init(ip, port, SERVER);
        return _socket->bindAndListen();
    } catch (std::exception &ex) {
        Logger::getInstance()->info(MSG_NO_ACCEPT_SOCKET);
        throw ex;
    }
}

void Server::acceptClients() {
     int retry = 1;

    for (int i = 0; i < clientNo && retry <= MAX_ACCEPT_RETRIES; i++, retry++) {
        try {
            auto * playerClient = new PlayerClient(_socket->accept(), &this->commandMutex, &this->commands);
            playerClient->name = i;
            clients.push_back(playerClient);
            pthread_create(&incomeThreads[i], nullptr, Server::handlePlayerClient, (void *) playerClient);
            pthread_create(&outcomeThreads[i], nullptr, Server::broadcastToPlayerClient, (void *) playerClient);
            pthread_mutex_lock(&logMutex);
            Logger::getInstance()->info(MSG_CLIENT_NUMBER_SERVER + std::to_string(i) + MSG_ACCEPTED_SERVER);
            pthread_mutex_unlock(&logMutex);
        } catch (std::exception &ex) {
            pthread_mutex_lock(&logMutex);
            Logger::getInstance()->error(MSG_CLIENT_NOT_ACCEPTED + std::to_string(i));
            pthread_mutex_unlock(&logMutex);
            i--;
        }
    }

    if (retry == MAX_ACCEPT_RETRIES && clients.size() < clientNo) {
        throw ServerException(MSG_ERROR_ACCEPT_CLIENTS);
    }
}

void * Server::handlePlayerClient(void * arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    pthread_mutex_t  * cmdMutex = playerClient->getCommandMutex();
    json msg;
    int msg_received;
    std::stringstream ss;

    while (playerClient->isConnected()) {
        msg_received = playerClient->receive(&msg);
        if (msg_received < 0) {
            continue;
        }
        if(!msg_received) {
            //TODO manejar cliente que cerró la conexión -> ¿lo borramos del array del server?
            continue;
        }

        pthread_mutex_lock(&logMutex);
        ss << "received user: " << playerClient->name << " "
           << "msg: " << msg.dump() << std::endl;
        Logger::getInstance()->info(ss.str());
        pthread_mutex_unlock(&logMutex);

        pthread_mutex_lock(cmdMutex);
        playerClient->commandQueue->push(msg);
        pthread_mutex_unlock(cmdMutex);
    }

    return nullptr;
}

void * Server::broadcastToPlayerClient(void *arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    std::queue<json> * outcomeQueue = &playerClient->outcome;
    pthread_mutex_t  * outMutex = playerClient->getOutcomeMutex();
    json msg;

    while (playerClient->isConnected()) {
        pthread_mutex_lock(outMutex);
        if (!outcomeQueue->empty()) {
            msg = outcomeQueue->front();
            outcomeQueue->pop();
        } else {
            pthread_mutex_unlock(outMutex);
            continue;
        }
        pthread_mutex_unlock(outMutex);

        pthread_mutex_lock(&logMutex);
        std::stringstream ss;
        ss << "envianding a user: " << playerClient->name << " "
           << std::endl << "val1: " << msg.dump() << std::endl;
        Logger::getInstance()->debug(ss.str());

        pthread_mutex_unlock(&logMutex);

        if(!playerClient->send(&msg)) {
            //ToDo handle error
            pthread_mutex_lock(&logMutex);
            Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
            pthread_mutex_lock(&logMutex);
        }
    }

    return nullptr;
}

// Infinite loop processing PlayerClients commands
bool Server::run() {
    pthread_mutex_t  * cmdMutex = &this->commandMutex;
    json msg;
    std::stringstream ss;

    //ToDo while (Game->isRunning()) {
    while (someoneIsConnected()) {
        pthread_mutex_lock(cmdMutex);
        if (!commands.empty()) {
            msg = commands.front();
            commands.pop();
        }
        pthread_mutex_unlock(cmdMutex);

        //ToDo change game state with msg

//        std::stringstream log;
//        log << "[main]" << clients.front()->isConnected();
//        Logger::getInstance()->debug(log.str());

        if (!msg.is_structured()) {
            continue;
        }

        pthread_mutex_lock(&logMutex);
        ss.str("");
        ss << "main: "
           << "msg: " << msg.dump() << std::endl;
        Logger::getInstance()->info(ss.str());
        pthread_mutex_unlock(&logMutex);

        msg = {4,5,6};

        for (auto & client : clients) {
            pthread_mutex_t * outMutex = client->getOutcomeMutex();
            pthread_mutex_lock(outMutex);
            client->outcome.push(msg);
            pthread_mutex_unlock(outMutex);
        }
    }

    for(int i=0; i < this->clientNo; i++){
        pthread_join(this->incomeThreads[i], nullptr);
    }

    for(int i=0; i < this->clientNo; i++){
        pthread_join(this->outcomeThreads[i], nullptr);
    }

    return true;
}

bool Server::someoneIsConnected() {
    for(auto & client : clients) {
        if(!client->isConnected()) {
            return false;
        }
    }
    return true;
}


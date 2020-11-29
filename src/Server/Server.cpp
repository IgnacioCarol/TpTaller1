#include "Server.h"
#include "ServerMsg.h"
#include <pthread.h>

Server* Server::instance = nullptr;

Server *Server::getInstance() {
    if (Server::instance == nullptr) {
        Server::instance = new Server();
    }

    return instance;
}

Server::~Server() {
    Logger::getInstance()->info(MSG_DESTROY_SERVER);
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

void Server::init(const char *ip, const char *port, int clientNo) {
    // Init threads
    this->clientNo = clientNo;
    this->incomeThreads = (pthread_t *) (malloc(sizeof(pthread_t) * clientNo));
    if (!this->incomeThreads) {
        Logger::getInstance()->error(MSG_NO_MEMORY_THREADS);
        throw ServerException(MSG_NO_MEMORY_THREADS);
    }

    this->outcomeThreads = (pthread_t *) (malloc(sizeof(pthread_t) * clientNo));
    if (!this->outcomeThreads) {
        Logger::getInstance()->error(MSG_NO_MEMORY_THREADS);
        throw ServerException(MSG_NO_MEMORY_THREADS);;
    }

    initSocket(ip, port);
    Logger::getInstance()->info(MSG_READY_SERVER);
    std::cout << MSG_READY_SERVER << std::endl; //TODO: Borrar
    acceptClients();
    Logger::getInstance()->info(MSG_ALL_CLIENTS_ACCEPTED_SERVER);
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
            clients.push_back(playerClient);
            pthread_create(&incomeThreads[i], nullptr, Server::handlePlayerClient, (void *) playerClient);
            pthread_create(&outcomeThreads[i], nullptr, Server::broadcastToPlayerClient, (void *) playerClient);
            Logger::getInstance()->info(MSG_CLIENT_NUMBER_SERVER + std::to_string(i) + MSG_ACCEPTED_SERVER);
        } catch (std::exception &ex) {
            Logger::getInstance()->error(MSG_CLIENT_NOT_ACCEPTED + std::to_string(i));
            i--;
        }
    }

    if (retry == MAX_ACCEPT_RETRIES && clients.size() < clientNo) {
        throw ServerException(MSG_ERROR_ACCEPT_CLIENTS);
    }
}

void * Server::handlePlayerClient(void * arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    pthread_mutex_t  * mutex = playerClient->getCommandMutex();
    msg_t msg;
    int msg_received;

    //ToDo while (playerClient->isConnected()) {
    while (true) {
        memset(&msg, 0, sizeof(msg));
        msg_received = playerClient->receive(&msg, sizeof(msg_t));
        if (msg_received < 0) {
            continue;
        }
        if(!msg_received) {
            //TODO manejar cliente que cerró la conexión -> ¿lo borramos del array del server?
        }
        pthread_mutex_lock(mutex);
        playerClient->commandQueue->push(msg);
        pthread_mutex_unlock(mutex);
    }

    return nullptr;
}

void * Server::broadcastToPlayerClient(void *arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    std::queue<msg_t> * outcomeQueue = &playerClient->outcome;
    pthread_mutex_t  * mutex = playerClient->getOutcomeMutex();
    msg_t msg;

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
        if(!playerClient->send(&msg, sizeof(msg_t))) {
            //ToDo handle error
            Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
        }
    }
    return nullptr;
}

// Infinite loop processing PlayerClients commands
bool Server::run() {

    //ToDo while (Game->isRunning()) {
    while (true) {
        pthread_mutex_t  * mutex = &this->commandMutex;
        msg_t message;
        memset(&message, 0, sizeof(message));

        pthread_mutex_lock(mutex);
        if (commands.size() > 0) {
            message = commands.front();
            commands.pop();
        }
        pthread_mutex_unlock(mutex);

        //ToDo change game state with msg

        if (message.val1 == 0) {
            continue;
        }

        std::stringstream ss;
        ss << "val1: " << message.val1 << std::endl
           << "val2: " << message.val2 << std::endl
           << "val3: " << message.val3 << std::endl
           << "val4: " << message.val4 << std::endl
           << "val5: " << message.val5 << std::endl
           << "val6: " << message.val6 << std::endl
           << "val7: " << message.val7 << std::endl;
        Logger::getInstance()->info(ss.str());

        message.val1+=10;
        message.val2+=10;
        message.val3+=10;
        message.val4+=10;
        message.val5+=10;
        message.val6+=10;
        message.val7+=10;

        for (auto & client : clients) {
            pthread_mutex_t * mutex = client->getOutcomeMutex();
            pthread_mutex_lock(mutex);
            client->outcome.push(message);
            pthread_mutex_unlock(mutex);
        }
    }

    return true;
}


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
            playerClient->name = i;
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

        ss << "received user: " << playerClient->name << " "
           << "msg: " << msg.dump() << std::endl;
        Logger::getInstance()->info(ss.str());

        //TODO: Ver si hay un mejor lugar para manejar los tipos de mensajes de entrada
        if (msg["message_type"] == LOGIN_MSG) {
            manageLogin(playerClient, msg);
        }

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

        std::stringstream ss;
        ss << "envianding a user: " << playerClient->name << " "
           << std::endl << "val1: " << msg.dump() << std::endl;
        Logger::getInstance()->debug(ss.str());

        if(!playerClient->send(&msg)) {
            //ToDo handle error
            Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
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

        ss.str("");
        ss << "main: "
           << "msg: " << msg.dump() << std::endl;
        Logger::getInstance()->info(ss.str());

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

void Server::manageLogin(PlayerClient* player, const json msg) {
    Players players = Config::getInstance()->getPlayers();
    pthread_mutex_t  * outMutex = player->getOutcomeMutex();
    json response = {
            {"response", UNAUTHORIZED}
    };
    std::string username = msg["username"];
    std::string password = msg["password"];

    //TODO: Validar cantidad de clientes.
    //if (clients.size() <= players.amount) {
    Logger::getInstance()->info("Will check all users for username: " + username +
    " and psw: " + password + ". Cantidad de users: " + std::to_string(players.users.size()));

    for (auto & user : players.users) {
        Logger::getInstance()->info("checking username: " + user.username + " and psw: " + user.password);
        if (user.username == username && user.password == password) {
            response = {
                    {"response", AUTHORIZED}
            };
        }
    }
    //}

    pthread_mutex_lock(outMutex);
    player->outcome.push(response);
    pthread_mutex_unlock(outMutex);
}


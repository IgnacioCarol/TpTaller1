#include "Server.h"

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
    pthread_mutex_destroy(&this->waitingRoomMutex);
    pthread_mutex_destroy(&this->clientsMutex);
    free(this->incomeThreads);
    free(this->outcomeThreads);
}

Server::Server() {
    pthread_mutex_init(&this->commandMutex, nullptr);
    pthread_mutex_init(&this->waitingRoomMutex, nullptr);
    pthread_mutex_init(&this->clientsMutex, nullptr);
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
        throw ServerException(MSG_NO_MEMORY_THREADS);
    }

    initSocket(ip, port);
    Logger::getInstance()->info(MSG_READY_SERVER);
    std::cout << MSG_READY_SERVER << std::endl;

    pthread_create(&this->acceptorThread, nullptr, Server::handleIncomingConnections, (void *) this);

//    try {
//        acceptClients();
//    } catch (std::exception &e) {
//        std::string error = e.what();
//        Logger::getInstance()->error("Failed to accept clients, error: " + error);
//    }
//
//    Logger::getInstance()->info(MSG_ALL_CLIENTS_ACCEPTED_SERVER);
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
            playerClient->id = i;
            addClient(playerClient);
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


void *Server::handleIncomingConnections(void *arg) {
    Server * server = (Server *)arg;
    int id = 0;
    std::stringstream ss;

    while(server->someoneIsConnected()) { //ToDo eso no va a funcionar, cambiar por algo asi while(server->isUpAndRunning()) o while true
        try {
            //ToDo hacer chequeo de cuantos confirmados hay, en caso de ya estar completos rechazar conexion entrante con JSON de rechazo
            auto * playerClient = new PlayerClient(server->_socket->accept(), &server->commandMutex, &server->commands);
            playerClient->id = id;
            // server->pushToWaitingRoom(playerClient);
            pthread_create(&server->loginThread, nullptr, Server::authenticatePlayerClient, (void *) playerClient);

            ss.str("");
            ss << "[thread:acceptor] Connection accepted with id: " << id << " move to login thread";
            Logger::getInstance()->info(ss.str());
        } catch (std::exception &ex) {
            Logger::getInstance()->error("Fatal error, couldn't accept client connection with id: " + std::to_string(id));
            id--;
        }

        id++; // Connection accepted moved to login thread
    }

    return nullptr;
}

void *Server::authenticatePlayerClient(void *arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    Players validPlayers = Config::getInstance()->getPlayers();
    std::string error;
    json response;
    bool authenticated = false;

    json msg = receive(playerClient);
    if (!(error = MessageValidator::validLoginMessage(msg)).empty()) {
        Logger::getInstance()->error("[Server - authenticate] unexpected login message from client: " + error);
        response = Protocol::buildErrorMsg(error);
        playerClient->pushOutcome(response);
        return nullptr;
    }

    std::string username = msg[MSG_CONTENT_PROTOCOL][MSG_LOGIN_USERNAME];
    std::string password = msg[MSG_CONTENT_PROTOCOL][MSG_LOGIN_PASSWORD];

    for (auto & user : validPlayers.users) {
        Logger::getInstance()->info("checking username: " + user.username + " and psw: " + user.password);
        if (user.username == username && user.password == password) {
            authenticated = true;
            break;
        }
    }

    Logger::getInstance()->debug("[Server] will send authentication message: " + std::string(authenticated ? "authorized" : "unauthorized"));
    response = Protocol::buildLoginMsgResponse(authenticated);
    playerClient->pushOutcome(response);
    return nullptr;
}

void * Server::handlePlayerClient(void * arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    json msg;
    std::stringstream ss;
    int tolerance = 0;

    while ((msg = receive(playerClient)) != nullptr) {
        ss.str("");
        ss << "[thread:listener]" << "[user:" << playerClient->id << "] "
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        playerClient->pushCommand(msg);
    }

    return nullptr;
}

json Server::receive(PlayerClient *playerClient) {
    Logger::getInstance()->debug("Receiving message from client " + std::to_string(playerClient->id));
    json msg;
    int msg_received;
    std::stringstream ss;
    int tolerance = 0;

    while (playerClient->isConnected()) {
        msg_received = playerClient->receive(&msg);
        if (msg_received < 0) {
            if (tolerance > 3) {//ToDo definir esto con mas criterio y poner en macro
                //ToDo suponemos que el socket se cerro, realizar tratamiento
                // 1. Marcar connected como false
                // 2. Mover player client a listado de conexiones muertas
                // comment: en caso de reconexion se marca connected como true y se mueve al listado de clients activo reanudando el juego para el client

                ss.str("");
                ss << "Fail tolerance exceeded! [thread:listener] " << "[user:" << playerClient->id << "] ";
                Logger::getInstance()->error(ss.str());
                throw ServerException(ss.str());
            }
            tolerance++;
            continue;
        }
        if (!msg_received) {
            //ToDo suponemos que el socket se cerro, realizar tratamiento
            // 1. Marcar connected como false
            // 2. Mover player client a listado de conexiones muertas
            // comment: en caso de reconexion se marca connected como true y se mueve al listado de clients activo reanudando el juego para el client
            ss.str("");
            ss << "Connection has been lost with client [thread:listener] " << "[user:" << playerClient->id << "] ";
            Logger::getInstance()->error(ss.str());
            throw ServerException(ss.str());
            //continue;
        }

        return msg;
    }

    return nullptr;
}

void * Server::broadcastToPlayerClient(void *arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    int tolerance = 0;
    json msg;

    while (playerClient->isConnected()) {
        msg = playerClient->getNewOutcomeMsg();
        if (msg.empty()) {
            continue;
        }

        std::stringstream ss;
        ss << "[thread:broadcast] " << "[user:" << playerClient->id << "] "
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        if(!playerClient->send(&msg)) {
            Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
            if (tolerance > 3) {//ToDo definir esto con mas criterio y poner en macro
                //ToDo suponemos que el socket se cerro, realizar tratamiento
                // 1. Marcar connected como false
                // 2. Mover player client a listado de conexiones muertas
                // comment: en caso de reconexion se marca connected como true y se mueve al listado de clients activo reanudando el juego para el client

                ss.str("");
                ss << "Fail tolerance exceeded! [thread:broadcast] " << "[user:" << playerClient->id << "] ";
                Logger::getInstance()->error(ss.str());
                throw ServerException(ss.str());
            }

            tolerance++;
            continue;
        }

        playerClient->popOutcome();
    }

    return nullptr;
}

// Infinite loop processing PlayerClients commands
bool Server::run() {
    pthread_mutex_t  * cmdMutex = &this->commandMutex;
    json msg;
    std::stringstream ss;

    //ToDo no empezar partida hasta que todos los jugadores esten conectados y autenticados

    for(int i=0; i < clients.size(); i++) {
        pthread_create(&incomeThreads[i], nullptr, Server::handlePlayerClient, (void *) clients[i]);
        pthread_create(&outcomeThreads[i], nullptr, Server::broadcastToPlayerClient, (void *) clients[i]);
    }

    //ToDo while (Game->isRunning()) {
    while (someoneIsConnected()) {
        msg = this->getNewCommandMsg();
        if (msg.empty()) {
            continue;
            //ToDo quiza no sea necesario saltear ya que el juego va a tener que seguir su curso (movimiento de enemigos, sprites, etc)
        }
        //ToDo change game state with msg

//        std::stringstream log;
//        log << "[main]" << clients.front()->isConnected();
//        Logger::getInstance()->debug(log.str());

        if (!msg.is_structured()) {
            continue;
        }

        ss.str("");
        ss << "[thread:run] " << "msg: " << msg.dump();
        Logger::getInstance()->info(ss.str());

        msg = {4,5,6,7};

        for (auto & client : clients) {
            client->pushOutcome(msg);
        }

        this->popCommand();
    }

    // Wait for all threads to finish before ending server run
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
        bool status = client->isConnected();
//        std::stringstream ss;
//        ss << "[user:" << client->name << "] status: " << (status ? "connected" : "disconnected");
//        Logger::getInstance()->debug(ss.str());
        if(!status) {
            return false;
        }
    }
    return true;
}

//TODO Borrar cuando esté listo
/*
void Server::manageLogin(PlayerClient* player, const json msg) {
    Players players = Config::getInstance()->getPlayers();
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

    player->pushOutcome(response);
}
*/

json Server::getNewCommandMsg() {
    pthread_mutex_lock(&this->commandMutex);
    json msg;
    if (this->commands.empty()) {
        pthread_mutex_unlock(&this->commandMutex);
        return json();
    }

    msg = this->commands.front();
    pthread_mutex_unlock(&this->commandMutex);
    return msg;
}

void Server::popCommand() {
    pthread_mutex_lock(&this->commandMutex);
    this->commands.pop();
    pthread_mutex_unlock(&this->commandMutex);
}

void Server::pushToWaitingRoom(PlayerClient * playerClient) {
    pthread_mutex_lock(&this->waitingRoomMutex);
    this->waitingRoom.push(playerClient);
    pthread_mutex_unlock(&this->waitingRoomMutex);
}

void Server::addClient(PlayerClient *player) {
    pthread_mutex_lock(&this->clientsMutex);
    this->clients.push_back(player);
    pthread_mutex_unlock(&this->clientsMutex);
}


#include "../Utils/CollisionsManager.h"
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
    pthread_mutex_destroy(&this->clientsMutex);
    pthread_mutex_destroy(&this->waitingRoomMutex);
    pthread_join(loginThread, nullptr);
    pthread_join(acceptorThread, nullptr);
}

Server::Server() {
    this->running = false;
    pthread_mutex_init(&this->commandMutex, nullptr);
    pthread_mutex_init(&this->clientsMutex, nullptr);
    pthread_mutex_init(&this->waitingRoomMutex, nullptr);
}

void Server::init(const char *ip, const char *port) {

    this->clientNo = Config::getInstance()->getPlayers().amount;

    initSocket(ip, port);
    this->running = true;
    Logger::getInstance()->info(MSG_READY_SERVER);
    std::cout << MSG_READY_SERVER << std::endl;

    pthread_create(&this->acceptorThread, nullptr, Server::handleIncomingConnections, (void *) this);
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

void *Server::handleIncomingConnections(void *arg) {
    Server * server = (Server *)arg;
    int id = 0;
    std::stringstream ss;

    while(server->isRunning()) {
        try {

            auto *playerClient = new PlayerClient(server->_socket->accept(), &server->commandMutex, &server->commands);
            playerClient->id = id;

            if (!server->validClientsMaximum(playerClient)) {
                delete playerClient;
                continue;
            }

            server->pushToWaitingRoom(playerClient);
            pthread_create(&server->loginThread, nullptr, Server::authenticatePlayerClient, (void *) server);

            ss.str("");
            ss << "[thread:acceptor] Connection accepted with id: " << id << " move to login thread";
            Logger::getInstance()->info(ss.str());
        } catch (std::exception &ex) {
            Logger::getInstance()->error("Fatal error, couldn't accept client connection with id: " + std::to_string(id) + ". ex: " + ex.what());
            id--;
        }

        id++; // Connection accepted moved to login thread
    }

    return nullptr;
}

void *Server::authenticatePlayerClient(void *arg) {
    Server * server = (Server *)arg;
    std::stringstream ss;

    while(server->waitingRoomIsEmpty()); //Wait for incoming playerClient, it should process only one playerClient
    PlayerClient * playerClient = server->popFromWaitingRoom();
    Players validPlayers = Config::getInstance()->getPlayers();
    std::string error;
    json response;
    bool authenticated = false;

    while (!authenticated && playerClient != nullptr && playerClient->isConnected()) {
        json msg = receive(playerClient);
        if (!(error = MessageValidator::validLoginMessage(msg)).empty()) {
            if (error == "No message received") {
                ss.str("");
                ss << "[Server][thread:login] Client socket closed, delete unauthenticated playerClient with id: " << playerClient->id;
                Logger::getInstance()->info(ss.str());
                delete playerClient;
                return nullptr;
            }

            Logger::getInstance()->error("[Server - authenticate] unexpected login message from client: " + error);
            response = ServerParser::buildErrorMsg(error);
            playerClient->pushOutcome(response);
            return nullptr;
        }

        std::string username = msg[MSG_CONTENT_PROTOCOL][MSG_LOGIN_USERNAME];
        std::string password = msg[MSG_CONTENT_PROTOCOL][MSG_LOGIN_PASSWORD];

        if (server->getClientsSize() >= server->clientNo && !server->clientHasLogged(username)) {
            playerClient->rejectConnection(MSG_RESPONSE_ERROR_SERVER_IS_FULL);
            Logger::getInstance()->error("[Server] Client " + username + " rejected because rooom is full");
            continue;
        }

        if (server->clientIsLogged(username)) {
            Logger::getInstance()->error("[Server] Client " + username + " is already logged. Rejecting client");
            playerClient->rejectConnection(MSG_RESPONSE_ERROR_USER_ALREADY_LOGGED);
            continue;
        }

        for (auto & user : validPlayers.users) {
            Logger::getInstance()->debug("checking username: " + user.username + " and psw: " + user.password);
            if (user.username == username && user.password == password) {
                authenticated = true;
                playerClient->username = username;
                server->addClient(playerClient);
                break;
            }
        }

        Logger::getInstance()->debug("[Server] will send authentication message: " + std::string(authenticated ? "authorized" : "unauthorized"));
        response = ServerParser::buildLoginMsgResponse(authenticated);

        if (!playerClient->send(&response)) {
            Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
            //TODO: ver si podemos tener reintentos acá
        }
        if (authenticated && GameServer::Instance()->isPlaying()) {
            Logger::getInstance()->info("Player " + playerClient->username + " is authenticating, put game is playing");
            json startGame = {{"startGame", true}};
            if (!playerClient->send(&startGame)) {
                Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
            }

            json initMsg = GameServer::Instance()->getInitializationMsg();
            if (!playerClient->send(&initMsg)) {
                Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
            }

            pthread_create(&server->incomeThreads[playerClient->username], nullptr, Server::handlePlayerClient, (void *) playerClient);
            pthread_create(&server->outcomeThreads[playerClient->username], nullptr, Server::broadcastToPlayerClient, (void *) playerClient);
        }
    }

    if (!authenticated && playerClient != nullptr && !playerClient->isConnected()) {
        ss.str("");
        ss << "[Server][thread:login] delete unauthenticated playerClient with id: " << playerClient->id;
        Logger::getInstance()->info(ss.str());
        delete playerClient;
    }

    return nullptr;
}

void * Server::handlePlayerClient(void * arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    json msg;
    std::stringstream ss;

    while (playerClient &&
            playerClient->isConnected() &&
            (msg = receive(playerClient)) != nullptr) {
        msg["username"] = playerClient->username;

        playerClient->pushCommand(msg);
    }

    Logger::getInstance()->info("Finishing handle player client thread");
    return nullptr;
}

json Server::receive(PlayerClient *playerClient) {
    Logger::getInstance()->debug("Receiving message from client " + playerClient->username);
    json msg;
    int msg_received;
    std::stringstream ss;

    while (playerClient->isConnected()) {
        Logger::getInstance()->debug("Waiting to receive msg from player: " + playerClient->username);
        msg_received = playerClient->receive(&msg);
        Logger::getInstance()->debug("Receive returned from player: " + playerClient->username);
        if (msg_received < 0) {
                ss.str("");
                ss << "Fail tolerance exceeded! [thread:listener] " << "[user:" << playerClient->id << "] ";
                Logger::getInstance()->error(ss.str());
        }
        if (!msg_received) {
            ss.str("");
            ss << "Connection has been lost with client [thread:listener] " << "[user:" << playerClient->id << "] ";
            Logger::getInstance()->error(ss.str());
        }

        return msg;
    }

    return nullptr;
}

void * Server::broadcastToPlayerClient(void *arg) {
    PlayerClient * playerClient = (PlayerClient *)arg;
    int tolerance = 0;
    json msg;
    std::stringstream ss;

    while (playerClient && playerClient->isConnected()) {
        msg = playerClient->getNewOutcomeMsg();
        if (msg.empty()) {
            continue;
        }

        ss.str("");
        ss << "[Server][thread:broadcast][event:queue_size][username:" << playerClient->username << "] outcome_size= " << playerClient->getOutcomeSize();
        Logger::getInstance()->debug(ss.str());

        ss.str("");
        ss << "[thread:broadcast] " << "[user:" << playerClient->id << "] "
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        if(!playerClient->send(&msg)) {
            Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
            if (tolerance > 3) {
                ss.str("");
                ss << "[thread:broadcast] " << "[user:" << playerClient->username << "] Fail tolerance exceeded! disconnecting user";
                Logger::getInstance()->error(ss.str());
                playerClient->disconnect();
            }

            tolerance++;
            continue;
        }

        playerClient->popOutcome();
        tolerance = 0;
    }

    Logger::getInstance()->info("Finishing broadcast to player client thread");
    return nullptr;
}

bool Server::run() {
    json msg;
    std::stringstream ss;

    while(this->getClientsSize() < this->clientNo);
    ss.str("");
    ss << "[thread:run] Amount of required clients reached successfully, initializing game...";
    Logger::getInstance()->info(ss.str());

    initThreads();
    json message = {{"startGame", true}};
    broadcast(message);
    GameServer* game = GameServer::Instance();
    if (!game->init(getClients())) {
        std::string error = "[Server] Couldnt initialize game server";
        Logger::getInstance()->error(error);
        throw ServerException(error);
    }

    clock_t t1;
    while (someoneIsConnected() && game->isPlaying()) {
        t1 = clock(); // Start transaction

        ss.str("");
        ss << "[Server][thread:run][event:queue_size] cmd_size= " << this->getCommandsSize();
        Logger::getInstance()->debug(ss.str());

        msg = this->getNewCommandMsg();
        if (!msg.empty()) {
            ss.str("");
            ss << "[thread:run] " << "msg: " << msg.dump();
            Logger::getInstance()->debug(ss.str());
            std::string username = msg["username"].get<std::string>();
            for (Player* player : game->getPlayers()) {
                if (player->getUsername() == username) {
                    std::vector<int> positions = {msg["up"].get<int>(), msg["left"].get<int>(), msg["down"].get<int>(), msg["right"].get<int>() };
                    player->move(positions);
                    if (msg["testMode"].get<int>()){
                        player->testMode();
                    }
                }
            }
            checkPlayersConnection();

            this->popCommand();
        }

        if (game->isPlaying()) {
            if (game->shouldSendScore() && !game->getScore()->isShowScoreTimeOver()) {
                msg = ServerParser::buildPartialScore(game->getPlayers(), game->getBackgroundStage());
            } else if (game->shouldSendScore() && game->getScore()->isShowScoreTimeOver()) {
                game->updateSendScore();
                msg = ServerParser::buildStopPartialScore();
            } else {
                game->updateGameObjects();
                game->updatePlayers();
                game->updateGameObjectsOnScreen();
                CollisionsManager::Instance()->checkCollisions(game->getGameObjectsOnScreen(), getPlayersAsGameObjects(game->getPlayers()));
                game->getCamera()->update(game->getPlayers());
                if (!game->isPlaying()) {
                    break;
                }
                msg = getPlayersPositionMessage();
            }
            broadcast(msg);
        }

        size_t waitTime = 0;
        while((float)(clock() - t1)/CLOCKS_PER_SEC < 0.017) {
            SDL_Delay(1);
            waitTime++;
        }
        ss.str("");
        ss << "[Server][thread:run] extra_time = " << waitTime;
        Logger::getInstance()->debug(ss.str());

    }

    Logger::getInstance()->info("Finished run loop");
    if (game->isTimeOver() || !game->arePlayersAlive()) {
        Logger::getInstance()->info("updating score players");
        game->updatePlayersScore();
    }

    while (someoneIsConnected() && !game->isPlaying()) {
        msg = ServerParser::buildGameOverMsg(game->getPlayers(), game->isTimeOver());
        broadcast(msg);
    }

    // Wait for all threads to finish before ending server run
    for(auto const& thread : incomeThreads) {
        pthread_join(thread.second, nullptr);
    }

    for(auto const& thread : outcomeThreads){
        pthread_join(thread.second, nullptr);
    }

    this->running = false;

    return true;
}

std::vector<GameObject*> Server::getPlayersAsGameObjects(const std::vector<Player*>& players) {
    std::vector<GameObject*> gos;
    for (auto player : players) {
        gos.push_back(player);
    }
    return gos;
}

bool Server::someoneIsConnected() {
    pthread_mutex_lock(&this->clientsMutex);
    for (auto & client : clients) {
        if (client->isConnected()) {
            pthread_mutex_unlock(&this->clientsMutex);
            return true;
        }
    }
    pthread_mutex_unlock(&this->clientsMutex);
    return false;
}

void Server::checkPlayersConnection() {
    pthread_mutex_lock(&this->clientsMutex);
    for (PlayerClient* client : clients) {
        if (client->isConnected()) {
            GameServer::Instance()->unpausePlayer(client);
        } else {
            GameServer::Instance()->pausePlayer(client);
        }
    }
    pthread_mutex_unlock(&this->clientsMutex);
}

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

bool Server::isRunning() {
    return this->running;
}

int Server::getClientsSize() {
    int size = 0;
    pthread_mutex_lock(&this->clientsMutex);
    size = this->clients.size();
    pthread_mutex_unlock(&this->clientsMutex);
    return size;
}

std::vector<PlayerClient *> Server::getClients() {
    std::vector<PlayerClient *> clients;
    pthread_mutex_lock(&this->clientsMutex);
    clients = this->clients;
    pthread_mutex_unlock(&this->clientsMutex);
    return clients;
}

int Server::getConnectedClientsSize() {
    int size = 0;
    pthread_mutex_lock(&this->clientsMutex);
    for (auto& client : clients) {
        if (client->isConnected()) {
            size++;
        }
    }
    pthread_mutex_unlock(&this->clientsMutex);
    return size;
}

bool Server::clientHasLogged(std::string username) {
    bool hasLogged = false;
    pthread_mutex_lock(&this->clientsMutex);
    for (auto& client: this->clients) {
        if (client->username == username) {
            hasLogged = true;
        }
    }
    pthread_mutex_unlock(&this->clientsMutex);
    return hasLogged;
}

bool Server::clientIsLogged(std::string username) {
    bool isLogged = false;
    pthread_mutex_lock(&this->clientsMutex);
    for (auto& client: this->clients) {
        if (client->username == username && client->isConnected()) {
            isLogged = true;
        }
    }
    pthread_mutex_unlock(&this->clientsMutex);
    return isLogged;
}

void Server::pushToWaitingRoom(PlayerClient *playerClient) {
    pthread_mutex_lock(&this->waitingRoomMutex);
    this->waitingRoom.push(playerClient);
    pthread_mutex_unlock(&this->waitingRoomMutex);
}

void Server::addClient(PlayerClient *player) {
    bool isAlreadyClient = false;
    int playerLoggedPos = 0;
    pthread_mutex_lock(&this->clientsMutex);
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i]->username == player->username) {
            isAlreadyClient = true;
            playerLoggedPos = i;
        }
    }
    if (isAlreadyClient) {
        clients.erase(clients.begin() + playerLoggedPos);
    }
    this->clients.push_back(player);
    pthread_mutex_unlock(&this->clientsMutex);
    if (isAlreadyClient) {
        Logger::getInstance()->info("[Server] Client " + player->username + " has lost connection and is loggin again.");
    }
}

PlayerClient *Server::popFromWaitingRoom() {
    PlayerClient * pc = nullptr;
    pthread_mutex_lock(&this->waitingRoomMutex);
    pc = this->waitingRoom.front();
    this->waitingRoom.pop();
    pthread_mutex_unlock(&this->waitingRoomMutex);
    return pc;
}


bool Server::waitingRoomIsEmpty() {
    bool result;
    pthread_mutex_lock(&this->waitingRoomMutex);
    result = this->waitingRoom.empty();
    pthread_mutex_unlock(&this->waitingRoomMutex);
    return result;
}


void Server::initThreads() {
    pthread_mutex_lock(&this->clientsMutex);
    for(auto& client: clients) {
        pthread_create(&incomeThreads[client->username], nullptr, Server::handlePlayerClient, (void *) client);
        pthread_create(&outcomeThreads[client->username], nullptr, Server::broadcastToPlayerClient, (void *) client);
    }
    pthread_mutex_unlock(&this->clientsMutex);
}

void Server::broadcast(json msg) {
    pthread_mutex_lock(&this->clientsMutex);
    for (auto & client : clients) {
        client->pushOutcome(msg);
    }
    pthread_mutex_unlock(&this->clientsMutex);
}

bool Server::validClientsMaximum(PlayerClient *playerClient) {
    int clientsSize = getConnectedClientsSize();
    std::stringstream ss;

    if (clientsSize >= this->clientNo) {
        playerClient->rejectConnection(MSG_RESPONSE_ERROR_SERVER_IS_FULL);

        if (clientsSize > this->clientNo) {
            ss.str("");
            ss << "[thread:acceptor] Fatal error, clients size is above allowed quantity";
            Logger::getInstance()->error(ss.str());
            throw ServerException(ss.str());
        }

        ss.str("");
        ss << "[thread:acceptor] server is full, playerClient with id: " << playerClient->id << " was rejected";
        Logger::getInstance()->info(ss.str());
        return false;
    }

    return true;
}

json Server::getPlayersPositionMessage() {
    GameServer* game = GameServer::Instance();
    if (game->changeLevel()){
        game->setChangeLevelFlag(false);
        return ServerParser::buildChangeLevelMsg(game->getGameObjects(), game->getBackgroundStage());
    }
    return ServerParser::buildPlayingGameMessage(game->getPlayers(), game->getGameObjects(), game->getCamera(),
                                                 game->getTimer());
}

size_t Server::getCommandsSize() {
    size_t result;
    pthread_mutex_lock(&this->commandMutex);
    result = this->commands.size();
    pthread_mutex_unlock(&this->commandMutex);
    return result;
}
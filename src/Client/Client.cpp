#include "Client.h"
#include "ClientParser.h"

using json = nlohmann::json;

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
    _login = new Login();
    pthread_mutex_init(&this->eventsMutex, nullptr);
    pthread_mutex_init(&this->commandsOutMutex, nullptr);
    keepConnection = true;

}

Client::~Client() {
    Logger::getInstance()->info(MSG_DESTROY_CLIENT);
    delete _socket;
    pthread_mutex_destroy(&this->eventsMutex);
    pthread_mutex_destroy(&this->commandsOutMutex);
}

void Client::init() {
    try {
        _socket->init(_IP, _port, CLIENT);
        _socket->connect();
        Logger::getInstance()->info(MSG_CONNECT_CLIENT);
        if (!_login->init()) {
            Logger::getInstance()->error(MSG_ERROR_INIT_LOGIN);
            throw ClientException(MSG_ERROR_INIT_LOGIN);
        }
    } catch (std::exception &ex) {
        Logger::getInstance()->error(MSG_CLIENT_NOT_INITIALIZED);
        throw ex;
    }
}

void Client::initThreads() {
    pthread_create(&incomeThread, nullptr, Client::handleServerEvents, (void *) this);
    pthread_create(&outcomeThread, nullptr, Client::broadcastToServer, (void *) this);
}

bool Client::login() {
    bool gamesIsInitiated = false;
    Logger::getInstance()->debug("Client start playing");
    try {
        while(!this->authenticate()) {} //TODO: Mejorar este while
        initThreads();
        SDL_Event e;
        _login->isWaitingRoom = true;
        while(_login->isWaitingRoom) {
            if (!this->eventsQueueIsEmpty()) {
                json receivedMessage = this->getMessageFromQueue();
                std::stringstream ss;
                ss <<"[Client] Message obtained at waiting stage:" << receivedMessage.dump();
                Logger::getInstance()->debug(ss.str());
                _login->isWaitingRoom = !(gamesIsInitiated |= receivedMessage["startGame"].get<bool>());
            }
            _login->showWaitingRoom(e);
        }
        delete _login;
    } catch(std::exception &ex) {
        Logger::getInstance()->error(MSG_CLIENT_ERROR_PLAYING + std::string(". Error: ") + ex.what());
        throw ex;
    }
    return gamesIsInitiated;
}

bool Client::authenticate() {
    Logger::getInstance()->debug("Client start authentication");
    Authentication *auth = _login->getAuthentication();
    username = auth->username;
    Logger::getInstance()->debug("Authentication login returned");
    std::stringstream ss;
    std::string error;

    json authJson = ClientParser::buildLoginMsg(auth->username, auth->password);

    Logger::getInstance()->debug("[Client] Will send authentication message");
    if (send(&authJson) < 0) {
        Logger::getInstance()->error(MSG_CLIENT_AUTH_SEND_ERROR);
        _login->showError("Unexpected error. Try again.");
        return false;
    }

    if (receive(&authJson) < 0) {
        Logger::getInstance()->error(MSG_CLIENT_AUTH_SEND_ERROR);
        _login->showError("Unexpected error. Try again.");
        return false;
    }

    if (!(error = MessageValidator::validLoginMessageResponse(authJson)).empty()) {
        Logger::getInstance()->error("[Client] unexpected login message response from server: " + error);
        _login->showError("Unexpected error. Try again.");
        return false;
    }

    if (authJson[MSG_STATUS_PROTOCOL] == 1) {
        error = authJson[MSG_ERROR_PROTOCOL].get<std::string>();
        Logger::getInstance()->error("[Client] unexpected response from server login: " + error);
        _login->showError(error);
        return false;
    } else if (authJson[MSG_CONTENT_PROTOCOL][MSG_RESPONSE_PROTOCOL] == MSG_LOGIN_AUTHORIZED) {
        Logger::getInstance()->info("Client authorized");
        return true;
    } else if (authJson[MSG_CONTENT_PROTOCOL][MSG_RESPONSE_PROTOCOL] == MSG_LOGIN_UNAUTHORIZED) {
        Logger::getInstance()->info("Client unauthorized");
        _login->showError("Invalid username or password");
        return false;
    } else {
        Logger::getInstance()->error("[Client] unexpected response from server login");
        _login->showError("Unexpected error. Try again.");
        return false;
    }
}

bool Client::isConnected() {
    return _socket->isConnected();
}

void Client::release() {
    if(!_socket->isConnected()) {
        return;
    }
    _socket->release();
    Logger::getInstance()->info(MSG_DISCONNECT_CLIENT);
}

int Client::send(json *msg) {
    return _socket->send(msg);
}

int Client::receive(json *msg) {
    Logger::getInstance()->debug("[Client] waiting to receive message");
    return _socket->receive(msg);
}

void * Client::handleServerEvents(void * arg) {
    auto * client = (Client *)arg;
    json msg;
    std::stringstream ss;
    while (client != nullptr &&
           client->isConnected() &&
           (msg = receive(client)) != nullptr && client->keepConnection) {
        ss.str("");
        ss << "[thread:Client]"
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());
        client->pushEvent(msg);
    }

    return nullptr;
}

json Client::receive(Client *client) {
    //Logger::getInstance()->debug("Receiving message from server.");
    json msg;
    int msg_received;
    std::stringstream ss;
    int tolerance = 0;

//    while (client != nullptr && client->isConnected() && client->keepConnection) {
    while (client->isConnected()) {
        msg_received = client->receive(&msg);
        if (msg_received < 0) {
            if (tolerance > 3) {
                ss.str("");
                ss << "Fail tolerance exceeded! [thread:listener]";
                Logger::getInstance()->error(ss.str());
                throw SocketException(ss.str());
            }
            tolerance++;
            continue;
        }
        if (!msg_received) {
            ss.str("");
            ss << "Connection has been lost with server [thread:listener]";
            Logger::getInstance()->error(ss.str());
            throw SocketException(ss.str());
            //continue;
        }

        return msg;
    }

    return nullptr;
}

json Client::getNewCommandMsg() {
    pthread_mutex_lock(&this->commandsOutMutex);
    json msg;
    if (this->commandsOut.empty()) {
        pthread_mutex_unlock(&this->commandsOutMutex);
        return json();
    }

    msg = this->commandsOut.front();
    pthread_mutex_unlock(&this->commandsOutMutex);
    return msg;
}

void Client::popCommandsOut() {
    pthread_mutex_lock(&this->commandsOutMutex);
    this->commandsOut.pop();
    pthread_mutex_unlock(&this->commandsOutMutex);
}

void * Client::broadcastToServer(void *arg) {
    auto * client = (Client *) arg;
    int tolerance = 0;
    json msg;
    while (client != nullptr && client->isConnected() && client->keepConnection) {
        msg = client->getNewCommandMsg();
        if (msg.empty()) {
            continue;
        }


        std::stringstream ss;
        ss << "[thread:broadcast]"
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        if(client->send(&msg)) {
            Logger::getInstance()->error("[Client] Error broadcasting msg to server");
            if (tolerance > 3) {
                ss.str("");
                ss << "Fail tolerance exceeded! [thread:broadcastClient]";
                Logger::getInstance()->error(ss.str());
                throw SocketException(ss.str());
            }

            tolerance++;
            continue;
        }
        client->popCommandsOut();
    }

    return nullptr;
}

void Client::run() {
    //Parser magico
    gameClient = GameClient::Instance();
    Logger::getInstance()->info("[Client:run] Game is playing: " + std::to_string(gameClient->isPlaying()));
    bool clientInitialized = false;
    std::stringstream ss;

    clock_t t2, t1 = clock();
    while (gameClient->isPlaying() && isConnected()) {
        t2 = clock();
        if ((t2 - t1) < 1000 * 300 / 60) {
            continue;
        }

        if (!this->eventsQueueIsEmpty()) {
            size_t result;
            pthread_mutex_lock(&this->eventsMutex);
            result = this->events.size();
            pthread_mutex_unlock(&this->eventsMutex);

            ss.str("");
            ss << "[Client][thread:run][event:queue_size] events_size= " << result;
            Logger::getInstance()->debug(ss.str());

            json receivedMessage = this->getMessageFromQueue();
            Logger::getInstance()->debug("[thread:run] msg: " + receivedMessage.dump());
            ProtocolCommand protocol = ClientParser::getCommand(receivedMessage);
            GameMsgParams initParams;
            GameMsgPlaying updateParams;
            GameMsgLevelChange updateLevel;

            switch(protocol) {
                case GAME_INITIALIZE_CMD:
                    initParams = ClientParser::parseInitParams(receivedMessage);
                    clientInitialized = gameClient->init(initParams, username.c_str());
                    if (!clientInitialized) { //le paso el resultado del parser magico
                        Logger::getInstance()->error("Error trying to init gameClient");
                        throw ClientException("Error trying to init gameClient");
                    }
                    break;
                case GAME_VIEW_CMD:
                    updateParams = ClientParser::parseUpdateParams(receivedMessage);
                    gameClient->update(updateParams); //le paso el resultado del parsermagico
                    break;
                case GAME_OVER_CMD:
                    gameClient->gameOver();
                    break;
                case GAME_CHANGE_LEVEL_CMD:
                    updateLevel = ClientParser::parseChangeLevelParams(receivedMessage);
                    gameClient -> changeLevel(updateLevel);
                    break;
                default:
                    std::stringstream ss;
                    ss << "[Client] unexpected protocol command. Protocol:" << protocol << " With received message" << receivedMessage.dump();
                    Logger::getInstance()->error(ss.str());
            }
        }
        if (clientInitialized) {
            gameClient->render();
            this->handleUserEvents();
        }

        t1 = clock();
    }
    keepConnection = false;
    pthread_join(incomeThread, nullptr); //FixMe creo que esto va a quedar trabado en el recv hasta que se cierre el socket de alguno de los dos lados
    pthread_join(outcomeThread, nullptr);
    gameClient -> clean();
    delete gameClient;
}

bool Client::eventsQueueIsEmpty() {
    bool result;
    pthread_mutex_lock(&this->eventsMutex);
    result = this->events.empty();
    pthread_mutex_unlock(&this->eventsMutex);
    return result;
}

json Client::getMessageFromQueue() {
    pthread_mutex_lock(&this->eventsMutex);
    json msg = events.front();
    events.pop();
    pthread_mutex_unlock(&this->eventsMutex);
    return msg;
}

void Client::pushEvent(json msg) {
    pthread_mutex_lock(&this->eventsMutex);
    this->events.push(msg);
    pthread_mutex_unlock(&this->eventsMutex);
}

void Client::pushCommand(json msg) {
    pthread_mutex_lock(&this->commandsOutMutex);
    this->commandsOut.push(msg);
    pthread_mutex_unlock(&this->commandsOutMutex);
}

void Client::handleUserEvents() {
    json msg;
    SDL_Event e;
    const Uint8 * keyboardState = SDL_GetKeyboardState( NULL );
    bool keysAssigned = false;
    int up, down, right, left;
    up = down = right = left = false;
    while( SDL_PollEvent( &e ) != 0 ) {
        if (e.type  == SDL_QUIT ) {
            GameClient::Instance()->gameOver();
            return;
        }
        if (!keysAssigned) {
            up = keyboardState[SDL_SCANCODE_UP];
            left = keyboardState[SDL_SCANCODE_LEFT];
            right = keyboardState[SDL_SCANCODE_RIGHT];
            down = keyboardState[SDL_SCANCODE_DOWN];
            keysAssigned = true;
        }
    }
    if (!keysAssigned) {
        return;
    }
    msg["up"] = up;
    msg["down"] = down;
    msg["left"] = left;
    msg["right"] = right;
    if (!(up || down || left || right)) {
        if (this->didMove) {
            pushCommand(msg);
            this->didMove = false;
        }

        return;
    }

    this->didMove = true;
    pushCommand(msg);
}
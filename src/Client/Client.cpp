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
        std::cout << MSG_CLIENT_NOT_INITIALIZED << " error: " << ex.what() <<std::endl;
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

    if (!isConnected()) {
        Logger::getInstance()->info(std::string("[Client] ") + TEXT_SERVER_DISCONNECTED_VALUE);
        _login->showError(TEXT_SERVER_DISCONNECTED_VALUE);
        return false;
    }

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
    try {
        while (client != nullptr &&
               client->isConnected() &&
               (msg = receive(client)) != nullptr && client->keepConnection) {
            ss.str("");
            ss << "[thread:Client]"
               << "msg: " << msg.dump();
            Logger::getInstance()->debug(ss.str());
            client->pushEvent(msg);
        }
    } catch (std::exception &ex) {
        Logger::getInstance()->error(std::string("[Client] Unexpected exception receiving message from server. Error: ") + ex.what());
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
    gameClient = GameClient::Instance();
    Logger::getInstance()->info("[Client:run] Game is playing: " + std::to_string(gameClient->isPlaying()));
    bool clientInitialized = false;
    bool serverDisconnected = false;
    std::stringstream ss;

    clock_t t1;
    while (gameClient->isPlaying()) {
        t1 = clock(); // Start transaction

        if (!this->eventsQueueIsEmpty()) {
            do {
                ss.str("");
                ss << "[Client][thread:run][event:queue_size] events_size= " << this->getEventsSize();
                Logger::getInstance()->debug(ss.str());

                json receivedMessage = this->getMessageFromQueue();
                Logger::getInstance()->debug("[thread:run] msg: " + receivedMessage.dump());
                ProtocolCommand protocol = ClientParser::getCommand(receivedMessage);
                GameMsgParams initParams;
                GameMsgPlaying updateParams;
                GameMsgLevelChange updateLevel;
                GameMsgShowPartialScore partialScoreParams;
                GameMsgShowGameOver gameOverParams;

                switch(protocol) {
                    case GAME_INITIALIZE_CMD:
                        initParams = ClientParser::parseInitParams(receivedMessage);
                        clientInitialized = gameClient->init(initParams, username.c_str());
                        if (!clientInitialized) {
                            Logger::getInstance()->error("Error trying to init gameClient");
                            throw ClientException("Error trying to init gameClient");
                        }
                        break;
                    case GAME_VIEW_CMD:
                        updateParams = ClientParser::parseUpdateParams(receivedMessage);
                        gameClient->update(updateParams);
                        break;
                    case GAME_OVER_CMD:
                        gameOverParams = ClientParser::parseGameOverParams(receivedMessage);
                        gameClient->gameOver(gameOverParams);
                        break;
                    case GAME_SHOW_PARTIAL_SCORE_CMD:
                        partialScoreParams = ClientParser::parsePartialScoreParams(receivedMessage);
                        gameClient -> showPartialScore(partialScoreParams);
                        break;
                    case GAME_STOP_PARTIAL_SCORE_CMD:
                        gameClient -> stopShowPartialScore();
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
            } while (this->getEventsSize() > 20 );
        }
        SDL_Event e;
        if (clientInitialized) {
            if (isConnected()) {
                gameClient->render();
                if (gameClient->isPlayerAlive()){
                    this->handleUserEvents();
                }
                else{
                    if (SDL_PollEvent(&e) != 0 && e.type == SDL_QUIT){
                        gameClient->stopPlaying();
                    }
                }
            } else {
                while( SDL_PollEvent( &e ) != 0 && gameClient->isPlaying()) {
                    if (e.type  == SDL_QUIT ) {
                        gameClient->stopPlaying();
                        break;
                    }

                    if (!serverDisconnected) {
                        Logger::getInstance()->info("Connection to the server was lost");
                        serverDisconnected = true;
                    }
                    gameClient->setServerDown();
                    gameClient->render();
                }
            }
        }

        size_t waitTime = 0;
        while((float)(clock() - t1)/CLOCKS_PER_SEC < 0.017) {
            SDL_Delay(1);
            waitTime++;
        }
        ss.str("");
        ss << "[Client][thread:run][user:" << this->username << "] extra_time =" << waitTime;
        Logger::getInstance()->debug(ss.str());
    }

    keepConnection = false;
    pthread_join(incomeThread, nullptr);
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
    int up, down, right, left, sound, music, testMode;
    up = down = right = left = false;
    while( SDL_PollEvent( &e ) != 0 ) {
        if (e.type  == SDL_QUIT ) {
            GameClient::Instance()->stopPlaying();
            return;
        }
        if (!keysAssigned) {
            up = keyboardState[SDL_SCANCODE_UP];
            left = keyboardState[SDL_SCANCODE_LEFT];
            right = keyboardState[SDL_SCANCODE_RIGHT];
            down = keyboardState[SDL_SCANCODE_DOWN];
            sound = keyboardState[SDL_SCANCODE_S];
            music = keyboardState[SDL_SCANCODE_M];
            testMode = keyboardState[SDL_SCANCODE_T];
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
    msg["testMode"] = testMode;

    if (music || sound){
        gameClient->pauseSoundEffects(music, sound);
    }

    if (!(up || down || left || right || testMode)) {
        if (this->didMove) {
            pushCommand(msg);
            this->didMove = false;
        }

        return;
    }

    this->didMove = true;
    pushCommand(msg);
}

size_t Client::getEventsSize() {
    size_t result;
    pthread_mutex_lock(&this->eventsMutex);
    result = this->events.size();
    pthread_mutex_unlock(&this->eventsMutex);
    return result;
}

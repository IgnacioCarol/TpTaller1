#include "Client.h"

using json = nlohmann::json;

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
    _login = new Login();
    pthread_mutex_init(&this->eventsMutex, nullptr);
}

Client::~Client() {
    Logger::getInstance()->info(MSG_DESTROY_CLIENT);
    delete _socket;
    pthread_mutex_destroy(&this->eventsMutex);
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
    pthread_create(&incomeThreads, nullptr, Client::handleServerEvents, (void *) this);
    pthread_create(&outcomeThreads, nullptr, Client::handleAndBroadcast, (void *) this);
}

void Client::login() {
    Logger::getInstance()->debug("Client start playing");
    try {
        while(!this->authenticate()) {} //TODO: Mejorar este while
        _login->showWaitingRoom();
        delete _login;
    } catch(std::exception &ex) {
        Logger::getInstance()->error(MSG_CLIENT_ERROR_PLAYING);
        throw ex;
    }
}

bool Client::authenticate() {
    Logger::getInstance()->debug("Client start authentication");
    Authentication *auth = _login->getAuthentication();
    Logger::getInstance()->debug("Authentication login returned");
    std::stringstream ss;
    std::string error;

    json authJson = Protocol::buildLoginMsg(auth->username, auth->password);

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
           (msg = receive(client)) != nullptr) {
        ss.str("");
        ss << "[thread:server]"
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        client->pushCommand(msg);
    }

    return nullptr;
}

json Client::receive(Client *client) {
    Logger::getInstance()->debug("Receiving message from server.");
    json msg;
    int msg_received;
    std::stringstream ss;
    int tolerance = 0;

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

void * Client::handleAndBroadcast(void *arg) {
    auto * client = (Client *) arg;
    int tolerance = 0;
    json msg;
    while (client != nullptr && client->isConnected()) {
        msg = handleUserEvents();
        if (msg.empty()) {
            continue;
        }

        std::stringstream ss;
        ss << "[thread:broadcast]"
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        if(client->send(&msg)) {
            Logger::getInstance()->error("[Client] Error broadcasting msg to client");
            if (tolerance > 3) {
                ss.str("");
                ss << "Fail tolerance exceeded! [thread:broadcast]";
                Logger::getInstance()->error(ss.str());
                throw SocketException(ss.str());
            }

            tolerance++;
            continue;
        }

    }

    return nullptr;
}

void Client::run() {
    initThreads();
    while (true || Game::Instance()->isPlaying()) { //Fixme condition is true because game->isplaying
        while (!this->eventsQueueIsEmpty()) {
            json receivedMessage = this->getMessageFromQueue();
            updateScreen(receivedMessage);
        }
        this->render();
    }
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

void Client::pushCommand(json msg) {
    pthread_mutex_lock(&this->eventsMutex);
    this->events.push(msg);
    pthread_mutex_unlock(&this->eventsMutex);
}

json Client::handleUserEvents() {
    json msg;
    SDL_Event e;
    bool up, down, right, left;
    up = down = right = left = false;
    while( SDL_PollEvent( &e ) != 0 ) {
        up = up || e.key.keysym.sym == SDLK_UP;
        left = left || e.key.keysym.sym == SDLK_LEFT;
        right = right || e.key.keysym.sym == SDLK_RIGHT;
        down = down || e.key.keysym.sym == SDLK_DOWN;
    }
    if (!(up || left || down || right)) {
        return json();
    }
    msg["up"] = up;
    msg["down"] = down;
    msg["left"] = left;
    msg["right"] = right;
    return msg;
}

void Client::popOutcome() {
    pthread_mutex_lock(&this->eventsMutex);
    this->events.pop();
    pthread_mutex_unlock(&this->eventsMutex);
}

void Client::updateScreen(json json) {

}

void Client::render() {

}

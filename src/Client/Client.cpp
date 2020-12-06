#include <src/Server/ServerMsg.h>
#include "Client.h"
#include "ServerClient.h"

using json = nlohmann::json;

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
    _login = new Login();
}

Client::~Client() {
    Logger::getInstance()->info(MSG_DESTROY_CLIENT);
    delete _socket;
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
        /*this->incomeThreads = (pthread_t *) malloc(sizeof(pthread_t));
        if (!this->incomeThreads) {
            Logger::getInstance()->error(MSG_NO_MEMORY_THREADS);
            throw SocketException(MSG_NO_MEMORY_THREADS);
        }

        this->outcomeThreads = (pthread_t *) malloc(sizeof(pthread_t));
        if (!this->outcomeThreads) {
            Logger::getInstance()->error(MSG_NO_MEMORY_THREADS);
            throw SocketException(MSG_NO_MEMORY_THREADS);
        }*/
    } catch (std::exception &ex) {
        Logger::getInstance()->error(MSG_CLIENT_NOT_INITIALIZED);
        throw ex;
    }
}

void Client::listenServer() {
    ServerClient* server = new ServerClient(this->_socket->accept(), &this->commandMutex, &this->commands);
    pthread_create(incomeThreads, nullptr, Client::handleServerClient, (void *) server);
    pthread_create(outcomeThreads, nullptr, Client::broadcastToServerClient, (void *) server);
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
        std::string error = authJson[MSG_ERROR_PROTOCOL].get<std::string>();
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

void Client::play() {

}

void * Client::handleServerClient(void * arg) {
    auto * serverClient = (ServerClient *)arg;
    json msg;
    std::stringstream ss;
    int tolerance = 0;

    while (serverClient != nullptr &&
           serverClient->isConnected() &&
           (msg = receive(serverClient)) != nullptr) {
        ss.str("");
        ss << "[thread:server]"
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        serverClient->pushCommand(msg);
    }

    return nullptr;
}

json Client::receive(ServerClient *serverClient) {
    Logger::getInstance()->debug("Receiving message from server.");
    json msg;
    int msg_received;
    std::stringstream ss;
    int tolerance = 0;

    while (serverClient->isConnected()) {
        msg_received = serverClient->receive(&msg);
        if (msg_received < 0) {
            if (tolerance > 3) {//ToDo definir esto con mas criterio y poner en macro
                //ToDo suponemos que el socket se cerro, realizar tratamiento
                // 1. Marcar connected como false
                // 2. Mover player client a listado de conexiones muertas
                // comment: en caso de reconexion se marca connected como true y se mueve al listado de clients activo reanudando el juego para el client

                ss.str("");
                ss << "Fail tolerance exceeded! [thread:listener]";
                Logger::getInstance()->error(ss.str());
                throw SocketException(ss.str());
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
            ss << "Connection has been lost with server [thread:listener]";
            Logger::getInstance()->error(ss.str());
            throw SocketException(ss.str());
            //continue;
        }

        return msg;
    }

    return nullptr;
}

void * Client::broadcastToServerClient(void *arg) {
    ServerClient * serverClient = (ServerClient *)arg;
    int tolerance = 0;
    json msg;

    while (serverClient != nullptr && serverClient->isConnected()) {
        msg = serverClient->getNewOutcomeMsg();
        if (msg.empty()) {
            continue;
        }

        std::stringstream ss;
        ss << "[thread:broadcast]"
           << "msg: " << msg.dump();
        Logger::getInstance()->debug(ss.str());

        if(!serverClient->send(&msg)) {
            Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
            if (tolerance > 3) {
                ss.str("");
                ss << "Fail tolerance exceeded! [thread:broadcast]";
                Logger::getInstance()->error(ss.str());
                throw SocketException(ss.str());
            }

            tolerance++;
            continue;
        }

        serverClient->popOutcome();
    }

    return nullptr;
}
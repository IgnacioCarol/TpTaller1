#include "Client.h"

using json = nlohmann::json;

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
    login = new Login();
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

        login->init();
    } catch (std::exception &ex) {
        Logger::getInstance()->error(MSG_CLIENT_NOT_INITIALIZED);
        throw ex;
    }
}

void Client::doLogin() {
    Logger::getInstance()->debug("Client start playing");
    try {
        while(!this->authenticate()) {} //TODO: Mejorar este while
        delete this->login;
    } catch(std::exception &ex) {
        Logger::getInstance()->error(MSG_CLIENT_ERROR_PLAYING);
        throw ex;
    }
}

bool Client::authenticate() {
    Logger::getInstance()->debug("Client start authentication");
    Authentication *auth = login->getAuthentication();
    Logger::getInstance()->debug("Authentication login returned");
    std::stringstream ss;

    json authJson = {
            {"username",     auth->username},
            {"password",     auth->password},
            {"message_type", LOGIN_MSG}
    };

    Logger::getInstance()->debug("Stoped setting json message");

    //TODO: Borrar, es para prueba
    ss.str("");
    ss << "authentication: "
       << "sending auth msg: " << authJson.dump() << std::endl;
    Logger::getInstance()->debug(ss.str());

    Logger::getInstance()->debug("Will send message");
    if (send(&authJson) < 0) {
        Logger::getInstance()->error(MSG_CLIENT_AUTH_SEND_ERROR);
        login->showError("Unexpected error. Try again.");
        return false;
    }

    if (receive(&authJson) < 0) {
        Logger::getInstance()->error(MSG_CLIENT_AUTH_SEND_ERROR);
        login->showError("Unexpected error. Try again.");
        return false;
    }

    if (authJson["response"] == AUTHORIZED) {
        Logger::getInstance()->info("Client authorized");
        return true;
    } else if (authJson["response"] == UNAUTHORIZED) {
        Logger::getInstance()->info("Client unauthorized");
        login->showError("Invalid username or password");
        return false;
    }
    return false;
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
    return _socket->receive(msg);
}

void Client::play() {

    json interactionJson;
    while (true) {
        if (receive(&interactionJson) < 0) {
            Logger::getInstance()->error("la cagaste pibe en el play");
            break;
        }
        //Lo que muestra el juego
        if (send(&interactionJson) < 0) {
            Logger::getInstance()->error("la cagaste pibe en el play send");
            break;
        }
    }
}





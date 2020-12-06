#include "Client.h"

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
//    delete login;
//    delete Game::Instance();
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

void Client::login() {
    Logger::getInstance()->debug("Client start playing");
    try {
        while(!this->authenticate()) {} //TODO: Mejorar este while
        _login->showWaitingRoom();
        delete _login;
        // Game::Instance()->play("./resources/config.xml"); //TODO: Deberia haber comunicacion con server para inicializar game
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

    //TODO: Borrar, es para prueba
    ss.str("");
    ss << "authentication: "
       << "sending auth msg: " << authJson.dump() << std::endl;
    Logger::getInstance()->debug(ss.str());

    Logger::getInstance()->debug("Will send message");
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





#include "Client.h"

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
    login = new Login();
}

Client::~Client() {
    Logger::getInstance()->info(MSG_DESTROY_CLIENT);
    delete _socket;
    delete login;
    delete Game::Instance();
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

void Client::play() {
    try {
        if (login->authenticate()){
            Game::Instance()->play("./resources/config.xml"); //TODO: Deberia haber comunicacion con server para inicializar game
        }
    } catch(std::exception &ex) {
        Logger::getInstance()->error(MSG_CLIENT_ERROR_PLAYING);
        throw ex;
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

int Client::send(msg_t *msg, size_t len) {
    return _socket->send(msg, len) == len;
}

int Client::receive(msg_t *msg, size_t len) {
    return  _socket->receive(msg, len) == len;
}





#include "Client.h"

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
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
    } catch (std::exception &ex) {
        Logger::getInstance()->error(MSG_NOT_CONNECT_CLIENT);
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





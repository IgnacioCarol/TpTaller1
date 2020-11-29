#include "Client.h"
#include "ClientMsg.h"
#include "../logger/logger.h"
using json = nlohmann::json;
Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
}

Client::~Client() {
    Logger::getInstance()->info(MSG_DESTROY_CLIENT);
    delete _socket;
}

bool Client::init() {
    _socket->init(_IP, _port, CLIENT);
    if(!_socket->connect()) {
        Logger::getInstance()->error(MSG_NOT_CONNECT_CLIENT);
        _socket->release();
        return false;
    }
    Logger::getInstance()->info(MSG_CONNECT_CLIENT);
    return true;
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
    int sent = _socket->send(msg);
    return sent;
}

bool Client::receive(json *msg) {
    int recv = _socket->receive(msg);
    return recv > 0;
}





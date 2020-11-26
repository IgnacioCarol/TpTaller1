#include "Client.h"
#include "src/logger/logger.h"

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
}

Client::~Client() {
    Logger::getInstance()->info("[Client] Destroying client");
    delete _socket;
}

void Client::init() {
    _socket->init(_IP, _port, CLIENT);
    if(!_socket->connect()) {
        _socket->release();
        return;
    }

    Logger::getInstance()->info("[Client] Client connected");
}

bool Client::isConnected() {
    return _socket->isConnected();
}

void Client::release() {
    if(!_socket->isConnected()) {
        return;
    }
    _socket->release();
    Logger::getInstance()->info("[Client] Client disconnected");
}

int Client::send(msg_t *msg, size_t len) {
    int sent = _socket->send(msg, len);
    return sent;
}

bool Client::receive(msg_t *msg, size_t len) {
    int recv = _socket->receive(msg, len);
    return recv > 0;
}





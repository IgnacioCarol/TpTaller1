#include "Client.h"
#include "logger/logger.h"

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
}

int Client::init() {
    _socket->init(_IP, _port, CLIENT);
    if(_socket->connect()) { //TODO: ver como manejar si el socket falla -> creo que habia preguntas sobre eso en el campus
        _socket->release();
    }
    Logger::getInstance()->info("Client connected");
    return 0;
}

bool Client::isConnected() {
    return _socket->isConnected();
}

void Client::release() {
    if(!_socket->isConnected()) {
        return;
    }
    _socket->release();
    Logger::getInstance()->info("Client disconnected");
}

bool Client::send(const void *msg, size_t len) {
    int sent = _socket->send(msg, len);
    return (sent == len);
}

bool Client::receive(const void *msg, size_t len) {
    int recv = _socket->receive(msg, len);
    return (recv == len);
}





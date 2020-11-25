#include "Client.h"
#include "logger/logger.h"

Client::Client(std::string IP, std::string port) {
    _IP = IP.c_str();
    _port = port.c_str();
    _socket = new Socket();
}

Client::~Client() {
    Logger::getInstance()->info("[Client] Destroying client");
    delete _socket;
}

int Client::init() {
    _socket->init(_IP, _port, CLIENT);
    if(!_socket->connect()) { //TODO: ver como manejar si el socket falla -> creo que habia preguntas sobre eso en el campus
        Logger::getInstance()->error("[Client] Failed connection to server");
        _socket->release();
        return 1;
    }

    Logger::getInstance()->info("[Client] Client connected");
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
    Logger::getInstance()->info("[Client] Client disconnected");
}

int Client::send(msg_t *msg) {
    int sent = _socket->send(msg);
    return sent;
}

bool Client::receive(msg_t *msg) {
    int recv = _socket->receive(msg);
    return recv > 0;
}





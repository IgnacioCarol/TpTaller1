#include "Client.h"
#include <src/logger/logger.h>


Logger* logger = Logger::getInstance();

Client::Client(const char * IP, const char * port) {
    _IP = IP;
    _port = port;
    _socket = Socket();
}

int Client::init() {
    _socket.init(_IP, _port, CLIENT);
    if(_socket.connect()) { //TODO: ver como manejar si el socket falla -> creo que habia preguntas sobre eso en el campus
        _socket.release();
    }
    logger -> info("Client connected");
    return 0;
}

bool Client::isConnected() {
    return _socket.isConnected();
}

void Client::release() {
    if(!_socket.isConnected()) {
        return;
    }
    _socket.release();
    logger -> info("Client disconnected");
}

bool Client::send(const void *msg, size_t len) {
    int sent = _socket.send(msg, len);
    return (sent == len);
}





#include "PlayerClient.h"

PlayerClient::PlayerClient(Socket *clientSocket, pthread_mutex_t * commandMutex, std::queue<msg_t> * commandQueue) {
    pthread_mutex_init(&this->outcomeMutex, nullptr);
    this->clientSocket = clientSocket;
    this->commandMutex = commandMutex;
    this->commandQueue = commandQueue;
}

PlayerClient::~PlayerClient() {
    delete this->clientSocket;
    pthread_mutex_destroy(&this->outcomeMutex);
}

Socket *PlayerClient::getSocket() {
    return this->clientSocket;
}

bool PlayerClient::receive(void* message, size_t len) {
   // msg_t message;
    memset(message, 0, len);

    if (this->clientSocket->receive(message, sizeof(msg_t)) < 0) { //ToDo aca verificar lo mismo, si recibo 0 bytes no deberia ser un error, ya que el cliente quiza no mando nada... creeeo verificar
        Logger::getInstance()->error("[Server] Couldn't receive message from client"); //TODO: se puede mejorar el log identificando el cliente
        //ToDo ver como handlear el error, si devolver excepcion o devolver msg_t * y devolver Null
        return false;
    }

    return true;
}

pthread_mutex_t *PlayerClient::getCommandMutex() {
    return this->commandMutex;
}

pthread_mutex_t *PlayerClient::getOutcomeMutex() {
    return &this->outcomeMutex;
}

bool PlayerClient::send(void * msg, size_t len) {
    int result = this->clientSocket->send(msg, len);
    return result == 0;
}

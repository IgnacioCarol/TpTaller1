//
// Created by Daniel Bizari on 24/11/2020.
//

#include "PlayerClient.h"

PlayerClient::PlayerClient(Socket *clientSocket, pthread_mutex_t * commandMutex, std::queue<msg_t *> * commandQueue) {
    pthread_mutex_init(this->outcomeMutex, nullptr);
    this->clientSocket = clientSocket;
    this->commandMutex = commandMutex;
    this->commandQueue = commandQueue;
}

PlayerClient::~PlayerClient() {
    delete this->clientSocket;
    pthread_mutex_destroy(this->outcomeMutex);
}

Socket *PlayerClient::getSocket() {
    return this->clientSocket;
}

msg_t PlayerClient::receive() {
    msg_t message;
    memset(&message, 0, sizeof(message));

    if (this->clientSocket->receive(&message, sizeof(message)) == 0) {
        Logger::getInstance()->error("[Server] Couldn't receive message from client"); //TODO: se puede mejorar el log identificando el cliente
        return false;
    }
}

pthread_mutex_t *PlayerClient::getCommandMutex() {
    return this->commandMutex;
}

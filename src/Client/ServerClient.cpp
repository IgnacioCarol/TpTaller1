//
// Created by nacho on 6/12/20.
//


#include <src/Server/ServerMsg.h>
#include "ServerClient.h"

ServerClient::ServerClient(Socket *clientSocket, pthread_mutex_t *commandMutex, std::queue<json> *commandQueue) {
    pthread_mutex_init(&this->outcomeMutex, nullptr);
    this->clientSocket = clientSocket;
    this->commandMutex = commandMutex;
    this->commandQueue = commandQueue;
}

ServerClient::~ServerClient() {
    delete this->clientSocket;
    pthread_mutex_destroy(&this->outcomeMutex);
}

int ServerClient::receive(json* message) {
    int received = this->clientSocket->receive(message);
    if (received < 0) { // There was an error
        Logger::getInstance()->error(MSG_ERROR_RECV_MSG_SERVER);
    } else if (!received) { // The client's socket was closed
        Logger::getInstance()->error(MSG_CLOSED_SOCKET_RECV_MSG_SERVER);
    }
    return received;
}

bool ServerClient::send(json *msg) {
    int result = this->clientSocket->send(msg);
    return result == 0;
}

bool ServerClient::isConnected() {
    return  this->clientSocket->isConnected();
}

void ServerClient::pushCommand(json msg) {
    pthread_mutex_lock(this->commandMutex);
    this->commandQueue->push(msg);
    pthread_mutex_unlock(this->commandMutex);
}

json ServerClient::getNewOutcomeMsg() {
    pthread_mutex_lock(&this->outcomeMutex);
    json msg;
    if (this->outcome.empty()) {
        pthread_mutex_unlock(&this->outcomeMutex);
        return json();
    }

    msg = this->outcome.front();
    pthread_mutex_unlock(&this->outcomeMutex);
    return msg;
}

void ServerClient::popOutcome() {
    pthread_mutex_lock(&this->outcomeMutex);
    this->outcome.pop();
    pthread_mutex_unlock(&this->outcomeMutex);
}

//
// Created by Daniel Bizari on 24/11/2020.
//

#include "PlayerClient.h"

PlayerClient::PlayerClient(Socket *clientSocket, pthread_mutex_t * commandMutex) {
    pthread_mutex_init(this->outcomeMutex, nullptr);
    this->clientSocket = clientSocket;
    this->commandMutex = commandMutex;
}

PlayerClient::~PlayerClient() {
    delete this->clientSocket;
    pthread_mutex_destroy(this->outcomeMutex);
}

Socket *PlayerClient::getSocket() {
    return this->clientSocket;
}

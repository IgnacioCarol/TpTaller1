#include "PlayerClient.h"
#include <json.hpp>
#include "ServerMsg.h"
#include <pthread.h>

using json = nlohmann::json;
PlayerClient::PlayerClient(Socket *clientSocket, pthread_mutex_t * commandMutex, std::queue<json> *commandQueue) {
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

int PlayerClient::receive(json* message) {
    int received = this->clientSocket->receive(message);
    if (received < 0) { // There was an error
        Logger::getInstance()->error(MSG_ERROR_RECV_MSG_SERVER);
    } else if (!received) { // The client's socket was closed
        Logger::getInstance()->error(MSG_CLOSED_SOCKET_RECV_MSG_SERVER);
    }
    return received;
}

pthread_mutex_t *PlayerClient::getCommandMutex() {
    return this->commandMutex;
}

pthread_mutex_t *PlayerClient::getOutcomeMutex() {
    return &this->outcomeMutex;
}

bool PlayerClient::send(json *msg) {
    int result = this->clientSocket->send(msg);
    return result == 0;
}

bool PlayerClient::isConnected() {
    return  this->clientSocket->isConnected();
}

void PlayerClient::lock() {
    pthread_mutex_lock(&this->outcomeMutex);
}

void PlayerClient::unlock() {
    pthread_mutex_unlock(&this->outcomeMutex);
}

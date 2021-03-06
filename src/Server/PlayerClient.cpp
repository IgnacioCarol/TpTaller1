#include "PlayerClient.h"
#include <json.hpp>
#include "ServerMsg.h"
#include <pthread.h>
#include <sstream>

using json = nlohmann::json;
PlayerClient::PlayerClient(Socket *clientSocket, pthread_mutex_t * commandMutex, std::queue<json> *commandQueue) {
    pthread_mutex_init(&this->outcomeMutex, nullptr);
    this->clientSocket = clientSocket;
    this->commandMutex = commandMutex;
    this->commandQueue = commandQueue;
    this->shouldBeConnected = true;
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

bool PlayerClient::send(json *msg) {
    int result = this->clientSocket->send(msg);
    return result == 0;
}

bool PlayerClient::isConnected() {
    return this->clientSocket->isConnected() && shouldBeConnected;
}

void PlayerClient::pushOutcome(json msg) {
    pthread_mutex_lock(&this->outcomeMutex);
    this->outcome.push(msg);
    pthread_mutex_unlock(&this->outcomeMutex);
}

json PlayerClient::getNewOutcomeMsg() {
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

void PlayerClient::popOutcome() {
    pthread_mutex_lock(&this->outcomeMutex);
    this->outcome.pop();
    pthread_mutex_unlock(&this->outcomeMutex);
}

void PlayerClient::pushCommand(json msg) {
    pthread_mutex_lock(this->commandMutex);
    this->commandQueue->push(msg);
    pthread_mutex_unlock(this->commandMutex);
}

void PlayerClient::rejectConnection(std::string error) {
    json msg = Protocol::buildErrorMsg(error);
    if (!send(&msg)) {
        Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
    }
}

size_t PlayerClient::getOutcomeSize() {
    size_t result;
    pthread_mutex_lock(this->commandMutex);
    result = this->outcome.size();
    pthread_mutex_unlock(this->commandMutex);
    return result;
}

void PlayerClient::disconnect() {
    clientSocket->release();
    shouldBeConnected = false;
}

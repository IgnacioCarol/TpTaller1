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
    bool status = this->clientSocket->isConnected();
//    std::stringstream ss;
//    ss << "[PlayerClient][user:" << this->name << "] status: " << (status ? "connected" : "disconnected");
//    Logger::getInstance()->debug(ss.str());
    return  status;
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

void PlayerClient::rejectConnection() {
    json msg = Protocol::buildErrorMsg(MSG_ERROR_SERVER_IS_FULL);
    if (!send(&msg)) {
        Logger::getInstance()->error(MSG_ERROR_BROADCASTING_SERVER);
    }
}

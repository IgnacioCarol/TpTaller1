#include "PlayerClient.h"
#include "ServerMsg.h"

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

int PlayerClient::receive(void* message, size_t len) {
    memset(message, 0, len);
    int received = this->clientSocket->receive(message, sizeof(msg_t));

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

bool PlayerClient::send(void * msg, size_t len) {
    int result = this->clientSocket->send(msg, len);
    return result == len;
}

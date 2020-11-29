//
// Created by Daniel Bizari on 24/11/2020.
//

#include "PlayerClient.h"
#include <lib/nlohmann/json.hpp>
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

json PlayerClient::receive() {
    json jsonMessage;
    if (this->clientSocket->receive(&jsonMessage) < 0) { //ToDo aca verificar lo mismo, si recibo 0 bytes no deberia ser un error, ya que el cliente quiza no mando nada... creeeo verificar
        Logger::getInstance()->error("[Server] Couldn't receive message from client"); //TODO: se puede mejorar el log identificando el cliente
        //ToDo ver como handlear el error, si devolver excepcion o devolver msg_t * y devolver Null
        return {};
    }
    return jsonMessage;
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

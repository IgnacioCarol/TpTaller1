//
// Created by Daniel Bizari on 24/11/2020.
//

#include "PlayerClient.h"

PlayerClient::PlayerClient(Socket *clientSocket) {
    this->clientSocket = clientSocket;
}

PlayerClient::~PlayerClient() {
    delete this->clientSocket;
}

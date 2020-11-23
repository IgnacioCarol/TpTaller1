//
// Created by Daniel Bizari on 23/11/2020.
//

#include "Server.h"

Server* Server::instance = nullptr;

Server *Server::getInstance() {
    if (Server::instance == nullptr) {
        Server::instance = new Server();
    }

    return instance;
}

Server::~Server() {

}

Server::Server() {

}

bool Server::run() {
    return false;
}



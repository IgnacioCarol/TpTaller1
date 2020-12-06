//
// Created by nacho on 6/12/20.
//

#ifndef TPTALLER1_SERVERCLIENT_H
#define TPTALLER1_SERVERCLIENT_H

#include <queue>
#include <src/Socket/Socket.h>
using json = nlohmann::json;

class ServerClient {
public:
    ServerClient(Socket *clientSocket, pthread_mutex_t * commandMutex, std::queue<json> *commandQueue);
    ~ServerClient();

    int receive(json *message);

    pthread_mutex_t *getCommandMutex();

    pthread_mutex_t *getOutcomeMutex();
    bool isConnected();
    void lock();
    void unlock();
    bool send(json *msg);
    void pushCommand(json msg);

    json getNewOutcomeMsg();

    void popOutcome();

private:
    Socket *clientSocket;
    pthread_mutex_t outcomeMutex;
    pthread_mutex_t *commandMutex;
    std::queue<json> *commandQueue;
    std::queue<json> outcome;

};


#endif //TPTALLER1_SERVERCLIENT_H

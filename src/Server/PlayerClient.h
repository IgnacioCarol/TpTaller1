//
// Created by Daniel Bizari on 24/11/2020.
//

#ifndef TPTALLER1_PLAYERCLIENT_H
#define TPTALLER1_PLAYERCLIENT_H
#include "../Socket/Socket.h"
#include <queue>

class PlayerClient {
public:
    explicit PlayerClient(Socket * clientSocket);
    Socket * getSocket();
    virtual ~PlayerClient();
private:
    Socket * clientSocket;
    pthread_mutex_t  commandMutex; // Mutex to control command queue
    pthread_mutex_t  outcomeMutex; // Mutex to control outcome queue
    std::queue<msg_t *> outcome; //ToDo define msg_t
};


#endif //TPTALLER1_PLAYERCLIENT_H

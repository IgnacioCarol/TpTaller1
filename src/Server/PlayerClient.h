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

    virtual ~PlayerClient();
private:
    Socket * clientSocket;
    std::queue<msg_t *> income; //ToDo define msg_t
    std::queue<msg_t *> outcome; //ToDo define msg_t
};


#endif //TPTALLER1_PLAYERCLIENT_H

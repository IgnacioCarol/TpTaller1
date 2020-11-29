//
// Created by Daniel Bizari on 24/11/2020.
//

#ifndef TPTALLER1_PLAYERCLIENT_H
#define TPTALLER1_PLAYERCLIENT_H
#include "../Socket/Socket.h"
#include <queue>
using json = nlohmann::json;
class PlayerClient {
public:
    explicit PlayerClient(Socket * clientSocket, pthread_mutex_t  * commandMutex, std::queue<json> *commandQueue);
    json receive();
    bool send(json *msg);
    pthread_mutex_t  * getCommandMutex();
    pthread_mutex_t  * getOutcomeMutex();
    Socket * getSocket();
    virtual ~PlayerClient();

    std::queue<json> * commandQueue; //ToDo define msg_t
    std::queue<json> outcome; //ToDo define msg_t
private:
    Socket * clientSocket;
    pthread_mutex_t  * commandMutex; // Mutex to control command queue
    pthread_mutex_t  outcomeMutex; // Mutex to control outcome queue

};


#endif //TPTALLER1_PLAYERCLIENT_H

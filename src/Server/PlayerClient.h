#ifndef TPTALLER1_PLAYERCLIENT_H
#define TPTALLER1_PLAYERCLIENT_H
#include "../Socket/Socket.h"
#include <queue>
using json = nlohmann::json;
class PlayerClient {
public:
    explicit PlayerClient(Socket * clientSocket, pthread_mutex_t  * commandMutex, std::queue<json> *commandQueue);
    int receive(json* message);
    bool send(json *msg);
    bool isConnected();
    void lock();
    void unlock();
    pthread_mutex_t  * getCommandMutex();
    pthread_mutex_t  * getOutcomeMutex();
    Socket * getSocket();
    virtual ~PlayerClient();

    std::queue<json> * commandQueue;
    std::queue<json> outcome;
    int name;
private:
    Socket * clientSocket;
    pthread_mutex_t  * commandMutex; // Mutex to control command queue
    pthread_mutex_t  outcomeMutex; // Mutex to control outcome queue

};


#endif //TPTALLER1_PLAYERCLIENT_H

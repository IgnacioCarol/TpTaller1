#ifndef TPTALLER1_PLAYERCLIENT_H
#define TPTALLER1_PLAYERCLIENT_H
#include "../Socket/Socket.h"
#include <queue>

class PlayerClient {
public:
    explicit PlayerClient(Socket * clientSocket, pthread_mutex_t  * commandMutex, std::queue<msg_t> * commandQueue);
    int receive(void* msg, size_t len); //ToDo tiene sentido mandar el len en el receive ?
    bool send(void * msg, size_t len);
    bool isConnected();
    void lock();
    void unlock();
    pthread_mutex_t  * getCommandMutex();
    pthread_mutex_t  * getOutcomeMutex();
    Socket * getSocket();
    virtual ~PlayerClient();

    std::queue<msg_t> * commandQueue; //ToDo define msg_t
    std::queue<msg_t> outcome; //ToDo define msg_t
private:
    Socket * clientSocket;
    pthread_mutex_t  * commandMutex; // Mutex to control command queue
    pthread_mutex_t  outcomeMutex; // Mutex to control outcome queue

};


#endif //TPTALLER1_PLAYERCLIENT_H

#ifndef TPTALLER1_PLAYERCLIENT_H
#define TPTALLER1_PLAYERCLIENT_H
#include "../Socket/Socket.h"
#include "../Protocol/Protocol.h"
#include <queue>
using json = nlohmann::json;
class PlayerClient {
public:
    explicit PlayerClient(Socket * clientSocket, pthread_mutex_t  * commandMutex, std::queue<json> *commandQueue);
    int receive(json* message);
    bool send(json *msg);
    void pushOutcome(json msg);
    json getNewOutcomeMsg();
    size_t getOutcomeSize();
    void popOutcome();
    void pushCommand(json msg);
    bool isConnected();
    void rejectConnection(std::string error);

    Socket * getSocket();
    virtual ~PlayerClient();

    int id;
    std::string username;
private:
    std::queue<json> * commandQueue;
    std::queue<json> outcome;
    Socket * clientSocket;
    pthread_mutex_t  * commandMutex; // Mutex to control command queue
    pthread_mutex_t  outcomeMutex; // Mutex to control outcome queue

};


#endif //TPTALLER1_PLAYERCLIENT_H

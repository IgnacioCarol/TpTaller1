#ifndef TPTALLER1_SERVER_H
#define TPTALLER1_SERVER_H

#include <sstream>
#include <iostream>
#include <exception>
#include <queue>
#include <pthread.h>
#include "../Socket/Socket.h"
#include "../logger/logger.h"
#include "ServerException.h"
#include "PlayerClient.h"
#include "ServerParser.h"
#include "../config/Config.h"
#include "../Protocol/MessageValidator.h"
#include "ServerMsg.h"
#include "GameServer.h"

class Server {

public:
    static Server * getInstance();
    virtual ~Server();

    void init(const char *ip, const char *port);
    bool run();
    bool isRunning();
    int getClientsSize();
    size_t getCommandsSize();
    int getConnectedClientsSize();
    std::vector<PlayerClient*> getClients();
    void pushToWaitingRoom(PlayerClient * playerClient);
    PlayerClient * popFromWaitingRoom();
    bool waitingRoomIsEmpty();
    void addClient(PlayerClient* player);
    void broadcast(json msg);

private:
    static Server * instance;

    Server();
    void initSocket(const char*ip, const char *port);
    json static receive(PlayerClient *playerClient);
    void initThreads();
    json getNewCommandMsg();
    void popCommand();
    //void acceptClients();
    bool someoneIsConnected();
    bool validClientsMaximum(PlayerClient *playerClient);
    bool clientIsLogged(std::string username);
    bool clientHasLogged(std::string username);
    void checkPlayersConnection();
    static void * authenticatePlayerClient(void * arg);
    static void * handlePlayerClient(void * arg);
    static void * handleIncomingConnections(void * arg);
    static void * broadcastToPlayerClient(void * arg);

    Socket *_socket;
    std::vector<PlayerClient *> clients;
    std::queue<json> commands; //ToDo por el momento puse de tipo msg_t pero deberían ser los comandos que recibe el server, mover arriba, abajo, izquierda, derecha
    std::queue<PlayerClient *> waitingRoom;
    pthread_mutex_t waitingRoomMutex; // Mutex to control waiting room queue
    pthread_mutex_t commandMutex; // Mutex to control command queue
    pthread_mutex_t clientsMutex; // Mutex to control clients vector
    pthread_t          acceptorThread;
    pthread_t          loginThread;
    std::map<std::string, pthread_t> incomeThreads;
    std::map<std::string, pthread_t> outcomeThreads;
    size_t             clientNo;
    bool               running;
    const int MAX_ACCEPT_RETRIES = 10;

    json getPlayersPositionMessage();

    static vector<GameObject *> getPlayersAsGameObjects(const vector<Player *> &players);
};


#endif //TPTALLER1_SERVER_H

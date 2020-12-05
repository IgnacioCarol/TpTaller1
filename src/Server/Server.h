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
#include "../Protocol.h"
#include "../config/Config.h"

class Server {

public:
    static Server * getInstance();
    virtual ~Server();

    void init(const char *ip, const char *port, int clientNo);
    bool run();
    bool isRunning();
    int getClientsSize();
    void addToClients(PlayerClient * playerClient);
    void pushToWaitingRoom(PlayerClient * playerClient);
    PlayerClient * popFromWaitingRoom();
    bool waitingRoomIsEmpty();

private:
    static Server * instance;
    Server();

    void initSocket(const char*ip, const char *port);
    json getNewCommandMsg();
    void popCommand();
    void acceptClients();
    bool someoneIsConnected();
    static void * authenticatePlayerClient(void * arg);
    static void * handlePlayerClient(void * arg);
    static void * handleIncomingConnections(void * arg);
    static void * broadcastToPlayerClient(void * arg);

    static void manageLogin(PlayerClient* player, const json msg); //TODO: Buscar algun lugar para manejar los eventos, quizas tener un login de parte del server

    Socket *_socket;
    std::vector<PlayerClient *> clients;
    std::queue<json> commands; //ToDo por el momento puse de tipo msg_t pero deberían ser los comandos que recibe el server, mover arriba, abajo, izquierda, derecha
    std::queue<PlayerClient *> waitingRoom;
    pthread_mutex_t waitingRoomMutex; // Mutex to control waiting room queue
    pthread_mutex_t commandMutex; // Mutex to control command queue
    pthread_mutex_t clientsMutex; // Mutex to control clients vector
    pthread_t          acceptorThread;
    pthread_t          loginThread;
    pthread_t        * incomeThreads;
    pthread_t        * outcomeThreads;
    size_t             clientNo;
    bool               running;
    const int MAX_ACCEPT_RETRIES = 10;
};


#endif //TPTALLER1_SERVER_H

#ifndef TPTALLER1_CLIENT_H
#define TPTALLER1_CLIENT_H

#include <string>
#include <exception>
#include <pthread.h>
#include <queue>
#include "json.hpp"
#include "../Socket/Socket.h"
#include "../logger/logger.h"
#include "../Login/Login.h"
#include "../Game.h"
#include "ClientMsg.h"
#include "ClientException.h"
#include "ClientParser.h"
#include "../Protocol/MessageValidator.h"
#include "GameClient.h"

using json = nlohmann::json;

class Client {
public:
    Client(std::string IP, std::string port);
    ~Client();
    void init();
    bool login();
    bool isConnected();
    int send(json *msg);
    int receive(json *msg);
    void release();


    void run();

private:
    const char * _IP;
    const char * _port;
    Socket* _socket;
    std::string _clientID; //TODO inicializar con el XML? Tal vez el ID pueda ser el nombre de usuario, una vez que se haga el login utilizar username en este campo
    Login* _login;
    std::string username;
    GameClient* gameClient;

    bool authenticate();
    pthread_t         incomeThread;
    pthread_t         outcomeThread;
    pthread_mutex_t eventsMutex;
    std::queue<json> events;
    pthread_mutex_t commandsOutMutex;
    std::queue<json> commandsOut;
    static void *handleServerEvents(void *arg);
    static json receive(Client *client);

    static void *broadcastToServer(void *arg);

    void initThreads();

    bool eventsQueueIsEmpty();

    json getMessageFromQueue();

    void pushEvent(json msg);

    void handleUserEvents();

    void pushCommand(json msg);

    json getNewCommandMsg();

    void popCommandsOut();

    bool keepConnection;
};


#endif //TPTALLER1_CLIENT_H

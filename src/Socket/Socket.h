#ifndef TPTALLER1_SOCKET_H
#define TPTALLER1_SOCKET_H

#include <string>
#include <netdb.h>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../logger/logger.h"
#include "SocketException.h"
#include "SocketMsg.h"
#include "json.hpp"
#include <csignal>


#ifdef __APPLE__
static const int FLAG_DATA_TRANSFER = 0;
#else
static const int FLAG_DATA_TRANSFER = MSG_NOSIGNAL;
#endif
enum ConnectionType {
    SERVER = 0,
    CLIENT = 1
};
// Struct de test para probar comunicacion de sockets
typedef struct {
    int val1;
    int val2;
    int val3;
    int val4;
    int val5;
    int val6;
    int val7;
} msg_t;

class Socket {
public:

    Socket();
    explicit Socket(int fd);
    //Initializes the private features
    //Gets the addresses that matches the IP and port specified
    void init(const char *IP, const char *port, ConnectionType type);

    //Opens a socket and connects to the server
    //Returns true or false whether the connection was established or not
    void connect();

    //Returns true if it is connected, false otherwise
    bool isConnected();

    //Returns the amount of bytes sent
    //If it returns 0 or a negative number, there was an error
    int send(nlohmann::basic_json<> *msg);

    //Returns the amount of bytes received
    //If it returns 0 or a negative number, there was an error
    int receive(nlohmann::basic_json<> *msg);

    //Closes the socket
    void release();

    // Bind server socket to local address and listen news. Throws exception if failed.
    void bindAndListen();

    // Accept incoming connection from a client, this is a blocking method.
    Socket * accept();

    virtual ~Socket();

private:
    int fd; //File descriptor
    ConnectionType _type;
    bool _connected;
    struct addrinfo * addresses;
    struct sockaddr_in server_addr;
    std::string port;

    template<class T>
    int send(T *data, int bytesToWrite);

    template<class T>
    int receive(T *msg, int receiveDataSize);
};


#endif //TPTALLER1_SOCKET_H

#ifndef TPTALLER1_SOCKET_H
#define TPTALLER1_SOCKET_H

#include <string>
#include <netdb.h>
#include <vector>

enum ConnectionType {
    SERVER = 0,
    CLIENT = 1
};

class Socket {
public:
    //Initializes the private features
    //Gets the addresses that matches the IP and port specified
    void init(const char *IP, const char *port, ConnectionType type);

    //Opens a socket and connects to the server
    //Returns true or false whether the connection was established or not
    bool connect();

    //Returns true if it is connected, false otherwise
    bool isConnected();

    //Returns the amount of bytes sent
    //If it returns 0 or a negative number, there was an error
    int send(const void* msg, size_t len);

    //Returns the amount of bytes received
    //If it returns 0 or a negative number, there was an error
    int receive(const void *msg, size_t len);

    //Closes the socket
    void release() const;

private:
    int fd; //File descriptor
    ConnectionType _type;
    bool _connected;
    struct addrinfo * addresses;

    //wrapper of send and receive functions
    int communication(ssize_t (* fx)(int __fd, void *__buf, size_t __n, int __flags), const void* msg, size_t len);
};


#endif //TPTALLER1_SOCKET_H

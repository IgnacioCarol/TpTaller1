#include "Socket.h"
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "../logger/logger.h"
#include "SocketException.h"

Socket::Socket() {
    // Create Socket
    // int socket(int domain, int type, int protocol);
    // Domain: AF_INET (IPv4 Internet protocols)
    // Type: SOCK_STREAM (Provides  sequenced,  reliable, two-way connection-based byte streams.)
    // Protocol: 0 (chosen automatically)
    fd = socket(AF_INET , SOCK_STREAM , 0);
    if (fd == -1) {
        Logger::getInstance()->error("Could not create socket");
        throw SocketException("Could not create socket");
    }

    Logger::getInstance()->info("Socket created");
}

void Socket::init(const char *IP, const char *port, ConnectionType type) {
    _connected = false;
    _type = type;
    this->port = port;

    int getaddr;
    struct addrinfo restrictions{};

    memset(&restrictions, 0, sizeof(struct addrinfo));
    restrictions.ai_family = AF_INET; //socket family
    restrictions.ai_socktype = SOCK_STREAM; //socket type
    restrictions.ai_flags = (type == SERVER) ? AI_PASSIVE : CLIENT;
    restrictions.ai_canonname = NULL;
    restrictions.ai_addr = NULL;
    restrictions.ai_next = NULL;

    getaddr = getaddrinfo(IP, port, &restrictions, &addresses);
    if (getaddr) {
        printf("%s\n", gai_strerror(getaddr)); //TODO ver cómo manejar errores de sockets
    }

    if (type == SERVER) {
        // Prepare the sockaddr_in structure
        /* Structures for handling internet addresses
        struct sockaddr_in {
            short            sin_family;   // e.g. AF_INET
            unsigned short   sin_port;     // e.g. htons(3490)
            struct in_addr   sin_addr;     // see struct in_addr, below
            char             sin_zero[8];  // zero this if you want to
        };
        struct in_addr {
            unsigned long s_addr;  // load with inet_aton()
        }; */
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY; //Address to accept any incoming messages. INADDR_ANY accepts any.
        server_addr.sin_port = htons(8080); // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
        //ToDo por el momento hardcodeo 8080, luego cambiar por port
    }
}

bool Socket::connect() {
    if (_type == SERVER) {
        Logger::getInstance()->error("Connect is only available for client side");
        return false;
    }

    int result;
    struct addrinfo *ptr;

    for (ptr = addresses; ptr && !_connected; ptr = ptr->ai_next) {
        fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(fd == -1) {
            continue;
        }
        result = ::connect(fd, ptr->ai_addr, ptr->ai_addrlen);
        if (result == -1) { //if the conecction fails, close the skt and seek for another one
            close(fd);
        } else {
            _connected = true;
        }
    }

    freeaddrinfo(addresses);
    return _connected;
}

void Socket::release() const {
    shutdown(fd, SHUT_RDWR);
    int return_value = close(fd);
    if (return_value == -1) {
       //TODO: manejar error de socket -> trycatch??
    }
}

bool Socket::isConnected() {
    return _connected;
}

int Socket::communication(ssize_t (* fx)(int __fd, void *__buf, size_t __n, int __flags), const void *msg, size_t len) {
    int total = 0; //Total bytes sent or received (depending on fx)
    int at_the_moment = 0; //Total bytes sent or received (depending on fx) at the moment
    auto *msg_to_send = (uint8_t*)msg;

    while(total < len) {
        at_the_moment = fx(fd, &msg_to_send[total], len - total, 0);
        if(at_the_moment == -1) {
            //TODO: manejar error
            break;
        }
        total += at_the_moment;
    }
    return total;
}

int Socket::send(const void *msg, size_t len) {
    return communication(reinterpret_cast<ssize_t (*)(int, void *, size_t, int)>(::send), msg, len);
}

int Socket::receive(const void *msg, size_t len) {
    return communication(::recv, msg, len);
}

bool Socket::bindAndListen() {
    if (_type == CLIENT) {
        Logger::getInstance()->error("Bind And Listen is only available for server side");
        return false;
    }

    // Bind
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    // sockfd -> file descriptor that refers to a socket
    // addr -> pointer to sockaddr_in structure of the SERVER
    // addrlen -> size of the sockaddr_in structure
    // bind() assigns the address specified by addr to the socket referred to by the file descriptor sockfd.
    if( bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        Logger::getInstance()->error("Bind failed");
        return false;
    }

    Logger::getInstance()->info("Bind success");

    // Listen
    // int listen(int sockfd, int backlog);
    // sockfd -> file descriptor that refers to a socket,in this case of type SOCK_STREAM
    // backlog-> The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
    // listen() marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept();
    if (listen(fd , 4) < 0) { //ToDo ver que pasa cuando mas usuarios se quieren conectar
        Logger::getInstance()->error("Listen failed");
        return false;
    }
    Logger::getInstance()->info("Listening on port: " + port + " Waiting for incoming connections...");
    return true;
}

Socket::~Socket() {
    close(fd);
}

Socket *Socket::accept() {
    int client_socket;
    struct sockaddr_in client_addr;
    int client_addrlen;

    // Accept incoming connection from a client
    // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    // sockfd -> socket that has been created with socket(), bound to a local address with bind(), and is listening for connections after a listen()
    // addr -> pointer to a sockaddr structure for the CLIENT.
    // addrlen -> size of sockaddr structure for the CLIENT.
    client_socket = ::accept(fd, (struct sockaddr *) &client_addr, (socklen_t*) &client_addrlen);
    if (client_socket < 0) {
        Logger::getInstance()->error("Accept failed");
        throw SocketException("Fail accepting client connection");
    }

    Socket * client = new Socket();
    client->setSocketFileDescriptor(client_socket);
    Logger::getInstance()->info("Connection accepted");

    return client;
}

void Socket::setSocketFileDescriptor(int fd) {
    this->fd = fd;
}


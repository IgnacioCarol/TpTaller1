#include "Socket.h"
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

Socket::Socket() {
}

void Socket::init(const char *IP, const char *port, ConnectionType type) {
    _connected = false;
    _type = type;

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
}

bool Socket::connect() {
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


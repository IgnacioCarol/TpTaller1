#include "Socket.h"

Socket::Socket() {
    // Create Socket
    // int socket(int domain, int type, int protocol);
    // Domain: AF_INET (IPv4 Internet protocols)
    // Type: SOCK_STREAM (Provides  sequenced,  reliable, two-way connection-based byte streams.)
    // Protocol: 0 (chosen automatically)
    fd = socket(AF_INET , SOCK_STREAM , 0);
    if (fd == -1) {
        Logger::getInstance()->error(MSG_SOCKET_CREATE_FAILED);
        throw SocketException(MSG_SOCKET_CREATE_FAILED);
    }

    Logger::getInstance()->info(MSG_SOCKET_CREATED);
}

Socket::Socket(int fd) {
    if (fd == -1) {
        Logger::getInstance()->error(MSG_SOCKET_INVALID_FILE_DESCRIPTOR);
        throw SocketException(MSG_SOCKET_INVALID_FILE_DESCRIPTOR);
    }

    this->fd = fd;
    Logger::getInstance()->info(MSG_SOCKET_CREATED);
}

void Socket::init(const char *IP, const char *port, ConnectionType type) {
    _connected = false;
    _type = type;
    this->port = port;

    // Prepare the server_addr structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    server_addr.sin_addr.s_addr = (type == SERVER) ? INADDR_ANY : inet_addr(IP);
    Logger::getInstance()->info(MSG_SOCKET_INITIALIZED);
}

void Socket::connect() {
    if (_type == SERVER) {
        Logger::getInstance()->error(MSG_SOCKET_CONNECT_SERVER);
        throw SocketException(MSG_SOCKET_CONNECT_SERVER);
    }

    //Connect to remote server
    // int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    // sockfd -> file descriptor that refers to a socket
    // addr -> pointer to sockaddr_in structure for the SERVER.
    // addrlen -> size of sockaddr_in structure for the SERVER.
    // The connect() system call connects the socket referred to by the file descriptor sockfd to the address specified by addr.
    if (::connect(fd , (struct sockaddr *)&server_addr , sizeof(struct sockaddr_in)) < 0){
        std::string error = MSG_SOCKET_CONNECT_FAILED + std::string(strerror(errno));
        Logger::getInstance()->error(error);
        throw SocketException(error);
    }
    _connected = true;
    Logger::getInstance()->info(MSG_SOCKET_CONNECTED);
}

void Socket::release() {
    shutdown(fd, SHUT_RDWR);
    if (close(fd) < 0) {
        Logger::getInstance()->error(MSG_SOCKET_CLOSE_FILE_FAILED + std::string(strerror(errno)));
    }
    _connected = false;
    Logger::getInstance()->info(MSG_SOCKET_RELEASED);
}

bool Socket::isConnected() {
    return _connected;
}

int Socket::send(void *msg, size_t len) {
    int total_bytes_written = 0;
    int bytes_written = 0;
    //int send_data_size = sizeof(msg_t);
    bool client_socket_still_open = true;
    auto *msg_to_send = (uint8_t*)msg;

    while ((len > total_bytes_written) && client_socket_still_open){
        bytes_written = ::send(fd, (msg_to_send + total_bytes_written), (len - total_bytes_written), 0);

        if (bytes_written < 0) { // Error
            Logger::getInstance()->error(MSG_SOCKET_SEND_FAILED + std::string(strerror(errno)));
            return bytes_written;
        }
        else if (bytes_written == 0) { // Socket closed
            Logger::getInstance()->error(MSG_SOCKET_CLOSED_SEND);
            client_socket_still_open = false;
        }
        else {
            total_bytes_written += bytes_written;
        }
    }

    return 0;
}

int Socket::receive(void *msg, size_t len) {
    int total_bytes_receive = 0;
    int bytes_received = 0;
    //int receive_data_size = sizeof(msg_t);
    bool client_socket_still_open = true;
    auto *msg_to_send = (uint8_t*)msg;

    // Receive
    // ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    // sockfd -> file descriptor that refers to a socket
    // buf -> where the received message into the buffer buf.
    // len -> The caller must specify the size of the buffer in len.
    // flags
    // The recv() call are used to receive messages from a socket.
    // If no messages are available at the socket, the receive call wait for a message to arrive. (Blocking)

    while ((len > bytes_received) && client_socket_still_open) {
        bytes_received = recv(fd, (msg_to_send + total_bytes_receive), (len - total_bytes_receive), 0);
        if (bytes_received < 0) { // Error
            Logger::getInstance()->error(MSG_SOCKET_RECEIVE_FAILED + std::string(strerror(errno)));
            return bytes_received;
        }
        else if (bytes_received == 0) { // Socket closed
            Logger::getInstance()->error(MSG_SOCKET_CLOSED_RECEIVE);
            client_socket_still_open = false;
        }
        else {
            total_bytes_receive += bytes_received;
        }
    }

    return total_bytes_receive;
}

void Socket::bindAndListen() {
    if (_type == CLIENT) {
        Logger::getInstance()->error(MSG_SOCKET_BIND_CLIENT_ERR);
        throw SocketException(MSG_SOCKET_BIND_CLIENT_ERR);
    }

    // Bind
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    // sockfd -> file descriptor that refers to a socket
    // addr -> pointer to sockaddr_in structure of the SERVER
    // addrlen -> size of the sockaddr_in structure
    // bind() assigns the address specified by addr to the socket referred to by the file descriptor sockfd.
    if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        Logger::getInstance()->error(MSG_SOCKET_BIND_FAILED);
        throw SocketException(MSG_SOCKET_BIND_FAILED);
    }

    Logger::getInstance()->info(MSG_SOCKET_BIND_SUCCESS);

    // Listen
    // int listen(int sockfd, int backlog);
    // sockfd -> file descriptor that refers to a socket,in this case of type SOCK_STREAM
    // backlog-> The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
    // listen() marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept();

    int res = listen(fd , 4); //If the queue is full (4 clients waiting) and another client tries to connect, it will be discarded.
    if (res < 0) {
        Logger::getInstance()->error(MSG_SOCKET_BIND_FAILED + std::string(strerror(errno)));
        throw SocketException(MSG_SOCKET_BIND_FAILED + std::string(strerror(errno)));
    }
    Logger::getInstance()->info("[Socket] Listening on port: " + port + ". Waiting for incoming connections...");
}

Socket::~Socket() {
    Logger::getInstance()->info(MSG_SOCKET_DESTROY);
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
        Logger::getInstance()->error(MSG_SOCKET_ACCEPT_FAILED);
        throw SocketException(MSG_SOCKET_ACCEPT_FAILED);
    }

    auto * client = new Socket(client_socket);
    Logger::getInstance()->info(MSG_SOCKET_CONNECTION_ACCEPTED);

    return client;
}


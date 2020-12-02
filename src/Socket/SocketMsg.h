#ifndef TPTALLER1_SOCKETMSG_H
#define TPTALLER1_SOCKETMSG_H

/* INFO */
#define MSG_SOCKET_BIND_SUCCESS "[Socket] Bind success "
#define MSG_SOCKET_CONNECTION_ACCEPTED "[Socket] Connection accepted. "
#define MSG_SOCKET_CREATED "[Socket] Socket created. "
#define MSG_SOCKET_INITIALIZED "[Socket] Socket initialized. "
#define MSG_SOCKET_CONNECTED "[Socket] Connected socket! "
#define MSG_SOCKET_RELEASED "[Socket] Socket released. "
#define MSG_SOCKET_DESTROY "[Socket] Destroying socket. "

/* ERRORS */
#define MSG_SOCKET_BIND_FAILED "[Socket] Bind failed "
#define MSG_SOCKET_BIND_CLIENT_ERR "[Socket] Bind And Listen is only available for server side "
#define MSG_SOCKET_ACCEPT_FAILED "[Socket] Failed accepting client connection. "
#define MSG_SOCKET_CLOSED_RECEIVE "[Socket] error trying to receive msg, socket closed. "
#define MSG_SOCKET_CLOSED_SEND "[Socket] error trying to send msg, socket closed. "
#define MSG_SOCKET_RECEIVE_FAILED "[Socket] unexpected error trying to receive msg. "
#define MSG_SOCKET_SEND_FAILED "[Socket] unexpected error trying to send msg. "
#define MSG_SOCKET_CLOSE_FILE_FAILED "[Socket] Close file descriptor failed. "
#define MSG_SOCKET_CONNECT_FAILED "[Socket] Connect failed. "
#define MSG_SOCKET_CONNECT_SERVER "[Socket] Connect is only available for client side. "
#define MSG_SOCKET_INVALID_FILE_DESCRIPTOR "[Socket] Invalid file descriptor, could not create socket. "
#define MSG_SOCKET_CREATE_FAILED "[Socket] Could not create socket. "


#endif //TPTALLER1_SOCKETMSG_H

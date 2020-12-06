#ifndef TPTALLER1_SERVERMSG_H
#define TPTALLER1_SERVERMSG_H


#define MSG_DESTROY_SERVER  "[Server] Destroying server"
#define MSG_NO_MEMORY_THREADS "[Server] Unable to create threads array, out of memory"
#define MSG_NO_ACCEPT_SOCKET "[Server] Server could not create the socket to accept to the clients"
#define MSG_READY_SERVER "[Server] Server is up and running"
#define MSG_ALL_CLIENTS_ACCEPTED_SERVER "[Server] All clients have been accepted"
#define MSG_CLIENT_NUMBER_SERVER "[Server] Client number "
#define MSG_ACCEPTED_SERVER " has been accepted"
#define MSG_CLIENT_NOT_ACCEPTED "[Server] Error accepting client number: "
#define MSG_ERROR_BROADCASTING_SERVER "[SERVER] Error broadcasting message to client"
#define MSG_ERROR_RECV_MSG_SERVER "[Server] Couldn't receive message from client because there was an error"
#define MSG_CLOSED_SOCKET_RECV_MSG_SERVER "[Server] Couldn't receive message from client because the communication was shut down"
#define MSG_ERROR_ACCEPT_CLIENTS "[Server] Error accepting all clients. Max retry achieved. "

#define MSG_ERROR_SERVER_IS_FULL "Room is full"

#define MSG_DISCONNECT_SERVER "[Server] Server disconnected"

#endif //TPTALLER1_SERVERMSG_H


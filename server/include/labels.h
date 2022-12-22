#ifndef SERVER_INCLUDE_LABELS_H_
#define SERVER_INCLUDE_LABELS_H_

// #define DATABASE_ADDRESS "tcp://127.0.0.1:31199/"
#define DATABASE_ADDRESS "tcp://127.0.0.1:3306/"
#define DATABASE_USERNAME "root"
#define DATABASE_PASSWORD ""

#define CLEAR_ALL           "\033[2J\033[H"

#define LISTEN_PORT 3333

#define BACKLOG 10
#define MAX_CLIENT 10

#define WAITING_NOTICE "Waiting for clients..."
#define CONNECTED_NOTICE "\t<!> New client connected\n"
#define TIMEOUT_NOTICE "Client Timeout !"
#define TIMEOUT_MSG "Connection Timeout !"
#define CLIENT_LABEL "Client: "

#define MAX_NAME_SIZE 20

#define START_APP 0

#define SIGN_UP 11
#define LOGIN 12

#define FIND_USERNAME 211
#define FIND_ROOMID 212

#define CHANGE_PASSWD 221
#define CHANGE_COLORS 222
#define CHANGE_FULLNM 223
    
#define GET_INFO 23

#define RETRIEVE_USER_MSGS 311
#define RETRIEVE_ROOM_MSGS 312
#define SEND_MESSAGE 32 

#define LOGOUT_USER 0

#define PUBLIC_ROOM true
#define PAIR_ROOM false

#endif // SERVER_INCLUDE_LABELS_H_

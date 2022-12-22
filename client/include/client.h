#ifndef __CLIENT_H_
#define __CLIENT_H_

#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <../external/nlohmann/json.hpp>
using json=nlohmann::json;

#include "io.h"

#define EMPTY_REQUEST "{}"
#define UNEXPECTED_RESPONSE "{\"msg\":\"UNEXP\",\"info\":{}}"
#define UNEXPECTED_RETRIEVE "{\"msg\":\"UNEXP\",\"info\":{\"history\":[],\"roomid\":-1}}"

enum Request {
    SIGN_UP = 11,
    LOGIN = 12,
    FIND_USER = 211,
    FIND_ROOM = 212,
    CHANGE_PASSWD = 221,
    CHANGE_COLORS = 222,
    CHANGE_FULLNM = 223,
    GET_INFO = 23,
    REMSGS_USER = 311,
    REMSGS_ROOM = 312,
    SEND_MSG = 32,
    LOGOUT = 0
};

class Client {
private:

    int socket_fd;
    sockaddr_in serv_addr;  // CONNECT TO SERVER

    char my_hashpass[HASH_PASS_LEN+1];

public:

    char  requestData[REQUEST_LEN+1];
    char responseData[RESPONSE_LEN+1];

    char my_username[USER_NAME_LEN+1];
    char my_fullname[FULL_NAME_LEN+1];

    char my_roomname[ROOM_NAME_LEN+1];
    Color my_color;

    bool chatting;
    bool sending;
    json retrieve;
    bool newmsg;

    Client();
    Client(int, Strings);
    
    void createSocket();
    int connectServer();

    void sendRequest(Request, json);
    int  recvResponse();
    void reconnectServer();

    json emptyRequest();

    json readResponse(Strings);
    json readRetrieve(Strings);

    json jsonSignUp(Strings, Strings, Strings);
    json jsonLogin(Strings, Strings);

    json jsonChangePass(Strings);
    json jsonChangeColor(int);
    json jsonChangeFullname(Strings);

    json jsonUserMsgs(Strings);
    json jsonRoomMsgs(RoomID);
    json jsonSendMessage(Strings, RoomID);

    void findRoomName(json, RoomID, char*);
    void findUserName(json, Strings, char*);

    void doFunction(int&, int&, int&);
    void resetAttribute();
    bool isLoggedIn();
    ~Client();
};

void printAllUser(json, Strings);
void printAllRoom(json);

void printAppInfo(json);

void printMessage(Strings, Strings, int, int, Strings, Strings);
void printAllMsgs(json, Strings);

#endif

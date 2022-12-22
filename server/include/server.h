#ifndef SERVER_INCLUDE_SERVER_H_
#define SERVER_INCLUDE_SERVER_H_

#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>

#include <time.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <stdlib.h>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
// #include "../external/cppconn/*"
#include "../external/nlohmann/json.hpp"

#include <iostream>
#include <string>
#include <map>
#include <vector>



#include "./labels.h"

using json = nlohmann::json;

struct User
{
    std::string username;
    std::string fullname;
    int id;
    int state;
    bool active;
    int color;
    int newest;

    User()
    {
        state = -1;
        id = -1;
        username = "not login";
        active = false;
        color = 0;
        newest = 0;
    }

    User(int __id, int __state)
    {
        id = __id;
        state = __state;
        username = "not login";
        active = false;
        color = 0;
        newest = 0;
    }

    void setState(int __state)
    {
        state = __state;
    }

    int getState()
    {
        return state;
    }
    

    bool isActive(){
        return active;
    }

    void activeUser(){
        active = true;
    }

    void deactiveUser(){
        active = false;
        id = -1;
    }
};


class Server
{
private:
    // User_data connfd;
    sockaddr_in address;
    pollfd fds[MAX_CLIENT + 1];
    std::map<int, User *> user;

    int listenfd, connfd;
    int addrlen;
    char recv_buffer[2048];
    char send_buffer[2048];
    int count_client = 0;
    int port;
    int size, newest;

    int nfds = 1, current_size = 0;
    int rc, len, on = 1;
    int close_conn = 0;
    bool compress_array = false;

    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

public:
    Server();
    int createSocket();
    int createAddress();
    int bindAddress();
    int configListen();
    int newListen();

    int setSendBuffer(User *__user, std::string __msg, json __infor);

    int handleUser(int __index_user);
    int checkJsonRequest( json __json_buffer, std::vector<std::string> __argv);

    int signUp(std::string __fullname, std::string __username, std::string __password);
    int logIn(std::string __username, std::string __password, User *__user);

    int activeUserDB(User *__user );
    int deactiveUser(User *__user );

    int saveMessage(User *__user, std::string __msg, int __id_room);

    User *createNewUser();
    json listAllActiveUsername();
    json listAllPublicRoom();
    json getAllPrivateMsg(User *__user, std::string  __other_username);
    json getAllRoomMsg(int __id_room);
    json getInfomation();

    // int updateUser(User * __user ,std::string __fullname, int __color);
    int updatePassword(User * __user, std::string __oldpwd, std::string __newpwd);
    int updateFullname(User * __user, std::string __fullname);
    int updateColor(User * __user, int __color);

    int run();
};

#endif // SERVER_INCLUDE_SERVER_H_"

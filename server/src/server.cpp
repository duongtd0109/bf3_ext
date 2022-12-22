#include "../include/server.h"
using namespace std;
Server::Server()
{
    listenfd = -1;
    connfd = -1;
    addrlen = sizeof(address);
    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));
    memset(&address, 0, sizeof(address));
    memset(fds, 0, sizeof(fds));

    driver = get_driver_instance();

    con = driver->connect(DATABASE_ADDRESS, DATABASE_USERNAME, DATABASE_PASSWORD);
    con->setSchema("CHAT_APPLICATION");
    stmt = con->createStatement();
}

int Server::createSocket()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(on), sizeof(on));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<char *>(on), sizeof(on));
    return listenfd;
}

int Server::createAddress()
{
    try
    {
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(LISTEN_PORT);
        return 0;
    }
    catch (...)
    {
        return -1;
    }
}

int Server::bindAddress()
{
    return bind(listenfd, reinterpret_cast<sockaddr *>(&address), sizeof(address));
}

int Server::configListen()
{
    return listen(listenfd, BACKLOG);
}

int Server::newListen()
{
    cout << WAITING_NOTICE << endl;
    connfd = accept(listenfd, reinterpret_cast<sockaddr *>(&address), reinterpret_cast<socklen_t *>(&address));

    cout << CONNECTED_NOTICE << endl;
    return connfd;
}

int Server::setSendBuffer(User *__user, string __msg, json __infor)
{

    json objJson;
    objJson["msg"] = __msg;
    objJson["info"] = __infor;
    memcpy(send_buffer, objJson.dump().c_str(), sizeof(send_buffer));
    return 0;
}
// SQL sign up

int Server::signUp(string fullname, string username, string password)
{
    res = stmt->executeQuery(string("SELECT * FROM USER WHERE USER_USERNAME = '" + username + "';").c_str());
    if (res->rowsCount() > 0)
    {
        return -1;
    }

    pstmt = con->prepareStatement("INSERT INTO USER (USER_FULLNAME, USER_USERNAME, USER_PASSWORD, USER_COLOR, USER_STATUS, USER_TIMESTAMP) VALUES ( ?, ?, ?, ? , ?, NOW() )");
    pstmt->setString(1, fullname);
    pstmt->setString(2, username);
    pstmt->setString(3, password);
    pstmt->setInt(4, 39);
    pstmt->setBoolean(5, false);
    pstmt->executeUpdate();

    return 0;
}

// SQL Login
int Server::logIn(string username, string password, User *__user)
{
    res = stmt->executeQuery(string("SELECT * FROM USER WHERE USER_USERNAME = '" + username + "' AND USER_PASSWORD = '" + password + "';").c_str());
    if (res->rowsCount() > 0)
    {
        res->next();
        if (username.compare(res->getString("USER_USERNAME"))==0)
        {
            __user->id = res->getInt("USER_ID");
            __user->username = res->getString("USER_USERNAME");
            __user->color = res->getInt("USER_COLOR");
            __user->fullname = res->getString("USER_FULLNAME");

            activeUserDB(__user);

            return 0;
        }
    }
    return -1;
}

int Server::activeUserDB(User *__user)
{
    pstmt = con->prepareStatement("UPDATE USER SET USER_STATUS = ? WHERE USER_ID = ? ;");
    pstmt->setInt(1, 1);
    pstmt->setInt(2, __user->id);
    pstmt->executeUpdate();
    __user->activeUser();
    return 0;
}

int Server::deactiveUser(User *__user)
{
    pstmt = con->prepareStatement("UPDATE USER SET USER_STATUS = ? WHERE USER_ID = ? ;");
    pstmt->setBoolean(1, false);
    pstmt->setInt(2, __user->id);
    pstmt->executeUpdate();
    __user->deactiveUser();
    return 0;
}

// SQL message
int Server::saveMessage(User *__user, string __msg, int __id_room)
{
    pstmt = con->prepareStatement("INSERT INTO MSG (MSG_CONTENT, USER_ID, ROOM_ID, MSG_TIMESTAMP) VALUES (?, ?, ?, NOW())");
    pstmt->setString(1, __msg);
    pstmt->setInt(2, __user->id);
    pstmt->setInt(3, __id_room);
    pstmt->executeUpdate();

    return 0;
}

json Server::getAllRoomMsg(int __room_id)
{
    json objJson, msgJson;
    sql::ResultSet *temp_res;
    res = stmt->executeQuery("(SELECT MSG_ID, MSG_CONTENT, USER_ID FROM MSG WHERE ROOM_ID = " + to_string(__room_id) + " ORDER BY MSG_ID DESC LIMIT 10) ORDER BY MSG_ID ASC;");
    objJson = json::array();

    while (res->next())
    {
        msgJson["id"] = res->getInt(1);
        msgJson["text"] = res->getString(2);

        temp_res = stmt -> executeQuery(string("SELECT USER_USERNAME, USER_COLOR FROM USER WHERE USER_ID = '" + to_string(res->getInt(3)) + "' ;").c_str());
        temp_res -> next();

        msgJson["usrnm"] = temp_res->getString(1);
        msgJson["color"] = temp_res->getInt(2);
        objJson.push_back(msgJson);
    }
    return objJson;
}

json Server::listAllActiveUsername()
{

    json objJson, personJson;
    objJson = json::array();
    res = stmt->executeQuery("SELECT USER_USERNAME, USER_FULLNAME, USER_COLOR FROM USER;");
    while (res->next())
    {
        personJson["usrnm"] = res->getString(1);
        personJson["fullnm"] = res->getString(2);
        personJson["color"] = res->getInt(3);
        objJson.push_back(personJson);
    }

    return objJson;
}

json Server::listAllPublicRoom()
{
    json objJson = json::array(), roomJson;

    res = stmt->executeQuery("SELECT ROOM_ID, ROOM_NAME FROM ROOM WHERE ROOM_TYPE = TRUE;");

    while (res->next())
    {
        roomJson["roomid"] = res->getInt(1);
        roomJson["rname"] = res->getString(2);
        objJson.push_back(roomJson);
    }

    return objJson;
}

json Server::getAllPrivateMsg(User *__user, string __other_username)
{
    json objJson;
    bool validRoom = false;
    int typeRoom, roomID, other_id;
    sql::ResultSet *temp_res;

    res = stmt -> executeQuery(string("SELECT USER_ID FROM USER WHERE USER_USERNAME = '" +  __other_username + "'").c_str());

    if (res -> rowsCount() == 0){

        return objJson;
    }
    else {
        res -> next();
        other_id = res -> getInt(1);
    }

    res = stmt->executeQuery(string("SELECT ROOM_ID FROM REGISTRATION WHERE USER_ID = " + to_string(__user->id)
    + " AND ROOM_ID in ( SELECT ROOM_ID FROM REGISTRATION WHERE USER_ID = "
                                    + to_string(other_id) + ") ;").c_str());

    while (res->next())
    {
        temp_res = stmt->executeQuery(string("SELECT ROOM_TYPE FROM ROOM WHERE ROOM_ID = " + to_string(res->getInt(1)) + " ; ").c_str());
        temp_res->next();

        typeRoom = temp_res->getBoolean(1);

        if (typeRoom == PAIR_ROOM)
        {
            roomID = res->getInt(1);
            validRoom = true;
            break;
        }
    }

    if (validRoom == 1)
    {
        objJson["roomid"] = roomID;
        objJson["history"] = getAllRoomMsg(roomID);
    }
    else
    {
        pstmt = con->prepareStatement("INSERT INTO ROOM (ROOM_NAME, ROOM_TYPE, ROOM_TIMESTAMP) VALUES (?, ?, NOW());");
        pstmt->setString(1, string("Room of ID=" + to_string(__user->id) + " and ID=" + to_string(other_id)));
        pstmt->setBoolean(2, PAIR_ROOM);
        pstmt->executeUpdate();

        res = stmt->executeQuery("SELECT LAST_INSERT_ID();");
        res->next();
        objJson["roomid"] = res->getInt(1);
        objJson["history"] = json::array();

        pstmt = con->prepareStatement("INSERT INTO REGISTRATION (USER_ID, ROOM_ID, REG_TIMESTAMP) VALUES (?, ?, NOW());");
        pstmt->setInt(1, __user->id);
        pstmt->setInt(2, objJson["roomid"]);
        pstmt->executeUpdate();

        pstmt = con->prepareStatement("INSERT INTO REGISTRATION (USER_ID, ROOM_ID, REG_TIMESTAMP) VALUES (?, ?, NOW());");
        pstmt->setInt(1, other_id);
        pstmt->setInt(2, objJson["roomid"]);
        pstmt->executeUpdate();
    }
    
    return objJson;
}

json Server::getInfomation(){

    json objJson, msg;
    objJson = json::array();

    msg["text"] = "When start app, you must sign up or login."; objJson.push_back(msg);
    msg["text"] = "If you sign up successful, then you login.";objJson.push_back(msg);
    msg["text"] = "If you login successful, you are allowed to chat!";objJson.push_back(msg);
    msg["text"] = "If you want to cancel any actions, press ESC.";objJson.push_back(msg);
    msg["text"] = "If you want to close app anytime, press Ctrl-C or Ctrl-Z.";objJson.push_back(msg);

    return objJson;
}

// int Server::updateUser(User *__user, string __fullname, int __color)
// {
//     pstmt = con->prepareStatement("UPDATE USER SET USER_FULLNAME = ?, USER_COLOR = ? WHERE USER_ID = ? ;");
//     pstmt->setString(1, __fullname);
//     pstmt->setInt(2, __color);
//     pstmt->setInt(3, __user->id);
//     pstmt->executeUpdate();
//     return 0;
// }

int Server::updatePassword(User * __user, std::string __oldpwd, std::string __newpwd) {
    res = stmt->executeQuery(string("SELECT USER_PASSWORD FROM USER WHERE USER_ID = '" + to_string(__user->id) + "';").c_str());
    if (res->rowsCount() > 0)
    {
        res->next();
        if (res->getString(1).compare(__oldpwd) == 0)
        {
            pstmt = con->prepareStatement("UPDATE USER SET USER_PASSWORD = ? WHERE USER_ID = ? ;");
            pstmt->setString(1, __newpwd);
            pstmt->setInt(2, __user->id);
            pstmt->executeUpdate();
            return 0;
        }
        return -1;
    }
    return -1;
}
int Server::updateFullname(User * __user ,std::string __fullname) {
    pstmt = con->prepareStatement("UPDATE USER SET USER_FULLNAME = ? WHERE USER_ID = ? ;");
    pstmt->setString(1, __fullname);
    pstmt->setInt(2, __user->id);
    pstmt->executeUpdate();
    return 0;
}
int Server::updateColor(User * __user, int __color) {
    pstmt = con->prepareStatement("UPDATE USER SET USER_COLOR = ? WHERE USER_ID = ? ;");
    pstmt->setInt(1, __color);
    pstmt->setInt(2, __user->id);
    pstmt->executeUpdate();
    return 0;
}

// for test
User *Server::createNewUser()
{
    return new User();
}

int Server::checkJsonRequest(json __json_buffer, vector<string> __argv)
{

    for (auto element : __argv)
    {
        if (__json_buffer.find(element) == __json_buffer.end())
        {
            return -1;
        }
    }

    return 0;
}

int Server::handleUser(int __fd)
{

    User *temp_user = user[__fd];
    json objJson, temp_json;
    int check;

    try
    {
        objJson = json::parse(recv_buffer);
        if (objJson.find("code") == objJson.end() || objJson.find("msg") == objJson.end())
        {
            // setSendBuffer(temp_user, "Json not match", " ");
            return -1;
        }
    }
    catch (const std::exception &e)
    {
        // setSendBuffer(temp_user, "Not a json requests", " ");
        return -1;
    }
    // switch (objJson["ce"].getod<int>())
    switch (objJson["code"].get<int>())
    {
    case SIGN_UP:
    {
        check = signUp(objJson["msg"]["fullnm"], objJson["msg"]["usrnm"], objJson["msg"]["passw"]);
        if (check == 0)
        {
            setSendBuffer(temp_user, "OK", "{}");
        }
        else
        {
            setSendBuffer(temp_user, "EXITED USERNAME", "{}");
        }
    }
    break;

    case LOGIN:
    {
        check = logIn(objJson["msg"]["usrnm"], objJson["msg"]["passw"], temp_user);
        if (check != -1)
        {
            temp_json["fullnm"] = temp_user->fullname;
            temp_json["usrnm"] = temp_user->username;
            temp_json["color"] = temp_user->color;
            setSendBuffer(temp_user, "OK", temp_json);
        }
        else
        {
            setSendBuffer(temp_user, "ACCOUNT DON'T CORRECT", "{}");
        }
    }
    break;

    case FIND_USERNAME:
    {
        if (!temp_user -> active){break;}
        setSendBuffer(temp_user, "OK", listAllActiveUsername());
    }
    break;

    case FIND_ROOMID:
    {
        if (!temp_user -> active){break;}
        setSendBuffer(temp_user, "OK", listAllPublicRoom());
    }
    break;

    // case SETTINGS:
    // {
    //     if (!temp_user -> active){break;}
    //     updateUser(temp_user, objJson["msg"]["fullnm"], objJson["msg"]["color"]);
    //     setSendBuffer(temp_user, "OK", "{}");
    // }
    // break;

    case CHANGE_PASSWD:
    {
        if (!temp_user -> active){break;}
        check = updatePassword(temp_user, objJson["msg"]["oldpwd"], objJson["msg"]["newpwd"]);
        if (check != -1)
        {
            temp_json["passw"] = objJson["msg"]["newpwd"].get<std::string>();
            setSendBuffer(temp_user, "OK", temp_json);
        }
        else
        {
            setSendBuffer(temp_user, "PASSWORD NOT MATCH", "{}");
        }
    }
    break;

    case CHANGE_COLORS:
    {
        if (!temp_user -> active){break;}
        updateColor(temp_user, objJson["msg"]["color"]);
        temp_json["color"] = objJson["msg"]["color"].get<int>();
        setSendBuffer(temp_user, "OK", temp_json);
    }
    break;

    case CHANGE_FULLNM:
    {
        if (!temp_user -> active){break;}
        updateFullname(temp_user, objJson["msg"]["fullnm"]);
        temp_json["fullnm"] = objJson["msg"]["fullnm"].get<std::string>();
        setSendBuffer(temp_user, "OK", temp_json);
    }
    break;

    case RETRIEVE_USER_MSGS:
    {
        if (!temp_user -> active){break;}
        temp_json = getAllPrivateMsg(temp_user, objJson["msg"]["usrnm"]);
        size = temp_json["info"]["history"].size();
        if (size==0)
        {
            return -1;
        }
        else
        {
            newest = temp_json["info"]["history"][size-1]["id"].get<int>();
        }
        if (size!=0)
        {
            if (temp_user->newest == newest)
            {
                return -1;
            }
            temp_user->newest = temp_json["info"]["history"][size-1]["id"].get<int>();
        }
        setSendBuffer(temp_user, "OK", temp_json);
    }
    break;

    case RETRIEVE_ROOM_MSGS:
    {
        if (!temp_user -> active){break;}
        temp_json["roomid"] = objJson["msg"]["room_id"].get<int>();
        temp_json["history"] = getAllRoomMsg(objJson["msg"]["room_id"]);
        size = temp_json["history"].size();
        if (size==0)
        {
            return -1;
        }
        else
        {
            newest = temp_json["history"][size-1]["id"].get<int>();
        }
        if (size!=0)
        {
            if (temp_user->newest == newest)
            {
                return -1;
            }
            temp_user->newest = temp_json["history"][size-1]["id"].get<int>();
        }
        setSendBuffer(temp_user, "OK", temp_json);
    }
    break;

    case SEND_MESSAGE:
    {
        if (!temp_user -> active){break;}
        saveMessage(temp_user, objJson["msg"]["text"], objJson["msg"]["room_id"]);
        setSendBuffer(temp_user, "OK", "{}");
    }
    break;

    case GET_INFO:
    {

        if (!temp_user -> active){break;}
        setSendBuffer(temp_user, "OK", getInfomation());
    }
    break;

    case LOGOUT_USER:
    {
        if (!temp_user -> active){break;}
        deactiveUser(temp_user);
        return -1;
        // setSendBuffer(temp_user, "OK", " ");
    }
    break;

    default:
        break;
    }
    return 0;
}

int Server::run()
{
    json objJson;
    User *temp_user;
    this->createSocket();
    this->createAddress();
    this->bindAddress();
    this->configListen();
    fds[0].fd = listenfd;
    fds[0].events = POLLIN;
    cout << CLEAR_ALL << endl;

    cout << "██████╗ ███████╗██████╗     ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗"  << endl;
    cout << "██╔══██╗██╔════╝╚════██╗    ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗" << endl;
    cout << "██████╔╝█████╗   █████╔╝    ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝" << endl;
    cout << "██╔══██╗██╔══╝   ╚═══██╗    ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗" << endl;
    cout << "██████╔╝██║     ██████╔╝    ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║" << endl;
    cout << "╚═════╝ ╚═╝     ╚═════╝     ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝" << endl;
                                                                            
    cout << "\t-.-.-.-.-.-.-.-.-.- LISTENING CLIENTS -.-.-.-.-.-.-.-.-.-" << endl;
    while (1)
    {
        rc = poll(fds, nfds, 0);
        current_size = nfds;
        for (int i = 0; i < current_size; i++)
        {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].fd == listenfd)
            {
                if (count_client < MAX_CLIENT)
                {
                    cout << "\t\t=======>>>>>>>>> NEW CONNECTION" << endl;
                    connfd = accept(listenfd, reinterpret_cast<sockaddr *>(&address), reinterpret_cast<socklen_t *>(&addrlen));
                    temp_user = new User(nfds, START_APP);
                    count_client++;

                    // setSendBuffer(temp_user, "Connection Success", " ");
                    // send(connfd, send_buffer, sizeof(send_buffer), 0);

                    if (connfd < 0)
                    {
                        break;
                    }

                    user[connfd] = temp_user;
                    fds[nfds].fd = connfd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                }
                else
                {
                    cout << "---------------[O V E R L O A D]---------------" << endl;
                    connfd = accept(listenfd, reinterpret_cast<sockaddr *>(&address), reinterpret_cast<socklen_t *>(&addrlen));
                    temp_user = new User(nfds, START_APP);
                    // setSendBuffer(temp_user, "Limited clients, Try again", " ");
                    // send(connfd, send_buffer, sizeof(send_buffer), 0);
                    delete temp_user;
                }
            }
            else
            {
                
                close_conn = 0;
                rc = recv(fds[i].fd, recv_buffer, sizeof(recv_buffer), 0);
                cout << "------------------------------------------" << endl;
                cout << "Socket: " << fds[i].fd  << "|| Username: " << user[fds[i].fd] -> username << "|| id: " << user[fds[i].fd] -> id << endl;
                cout << "Recv :" << recv_buffer << endl;
                

                if (rc == 0)
                {
                    cout << "\t\t=======>>>>>>>>> 1 CONNECTION CLOSE" << endl;
                    close_conn = 1;
                }

                if (rc > 0)
                {


                    if (this->handleUser(fds[i].fd) == -1){
                        continue;
                    };
                    cout << "Send :" << send_buffer << endl;
                    send(fds[i].fd, send_buffer, sizeof(send_buffer), 0);
                }

                if (rc < 0)
                {
                    close_conn = 1;
                }

                if (close_conn == 1)
                {
                    deactiveUser(user[fds[i].fd]);
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    compress_array = true;
                    count_client--;
                }
            }
        }

        if (compress_array)
        {
            compress_array = false;
            for (int i = 0; i < nfds; i++)
            {
                if (fds[i].fd == -1)
                {
                    for (int j = i; j < nfds; j++)
                    {
                        fds[j].fd = fds[j + 1].fd;
                    }

                    i--;
                    nfds--;
                }
            }
        }
    }

    for (int i = 0; i < nfds; i++)
    {
        if (fds[i].fd >= 0)
        {
            close(fds[i].fd);
        }
    }
    return 0;
}

#include "../include/client.h"

char username[USER_NAME_LEN+1];
char password[PASS_WORD_LEN+1];
char fullname[FULL_NAME_LEN+1];
char roomname[ROOM_NAME_LEN+1];
char preusrnm[USER_NAME_LEN+1];

char msg[MESSAGE_LEN+1];
char info[MESSAGE_LEN+1];

json requestLogin;
json request;
json response;

int check;
int chatting_mode;
int setting_mode;
int helping_mode;
int confirm;

Color color;
RoomID room_id;

Client::Client() {

    socket_fd = -1;
    chatting = false;
    sending = false;

    resetAttribute();
}

Client::Client(int port_num, Strings ip_address) {

    socket_fd = -1;
    chatting = false;
    sending = false;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);

    resetAttribute();
}

void Client::createSocket() {
    std::cout << CLEAR_ALL;
    std::cout << std::endl;
    std::cout << "Creating socket ..." << std::endl;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("socket() failed");
        exit(-1);
    }
    else {
        printFormat("Create socket successful!\n", BOLD, GREEN, LEFT, 0);
    }
    
}

int Client::connectServer() {

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3333);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::cout << "Connecting to server ..." << std::flush;
    int retry = 10;
    int connectCheck = -1;
    while (connectCheck < 0) {
        connectCheck = connect(socket_fd, (sockaddr *)&serv_addr, sizeof(serv_addr));
        std::cout << '.' << std::flush;
        sleep(1);
        retry--;
        if (retry<0) {
            return -1;
        }
    }
    std::cout << std::endl;
    printFormat("The connection is accepted!\n", BOLD, GREEN, LEFT, 0);
    printCountdown("Initializing app", COUNTDOWN_SEC);
    return 0;
}

void Client::sendRequest(Request code, json msg) {

    json file;
    file["code"] = code;
    file["msg"] = msg;

    rstString(requestData, REQUEST_LEN);
    cpyString(requestData, REQUEST_LEN, file.dump().c_str());
    
    sending = true;
    send(socket_fd, requestData, REQUEST_LEN, 0);
    if (code==LOGOUT) {
        sending = false;
        return;
    }
    while (true) {
        check = recvResponse();

        if (check<0) {
            printFormat("\n<ERROR> Receiving unsuccessful!\n", RED, ITALIC, LEFT, 0);
            printCountdown("\nTry again ...", COUNTDOWN_SEC);
            continue;
        }
        else if (check==0) {
            printFormat("\nDisconnect! Try to reconnect ...\n", RED, ITALIC, LEFT, 0);
            reconnectServer();
            continue;
        }
        else {
            break;
        }
    }
    sending = false;
}

char responseBuffer[RESPONSE_LEN];
int Client::recvResponse() {

    rstString(responseData, RESPONSE_LEN);

    int totallength=0;
    int recvlength=0;
    while (totallength<2048) {
        rstString(responseBuffer, RESPONSE_LEN);
        recvlength = recv(socket_fd, responseBuffer, RESPONSE_LEN, 0);
        if (recvlength<=0) {
            return recvlength;
        }
        sprintf(responseData+totallength, "%s", responseBuffer);
        totallength += recvlength;
    }
    return 1;
}

void Client::reconnectServer() {

    chatting = false;
    while (true) {
        // CLOSE THE CURRENT SOCKET
        if (socket_fd > 0) {
            close(socket_fd);
            socket_fd = -1;
        }
        createSocket();

        // WHEN SERVER HAVE NOT ACCEPTED YET
        check = connectServer();
        if (check != -1) {
            break;
        }
    }

    if (isLoggedIn()==false) {
        return;
    }
    // WHEN USER HAS LOGGED IN SUCCESSFULLY
    requestLogin["usrnm"] = my_username;
    requestLogin["passw"] = my_hashpass;

    int receiveCheck = -1;
    while (receiveCheck <= 0) {
        sendRequest(LOGIN, requestLogin);

        json response = readResponse(responseData);
        if (cmpString(response["msg"].get<std::string>().c_str(), "OK")==true) {
            break;
        }
        else {
            perror("database crash");
            exit(1);
        }
    }
    
}

json Client::emptyRequest() {
    json file = json::parse(EMPTY_REQUEST);
    return file;
}

json Client::readResponse(Strings response) {

    if (response[0]!=0) {
        json file = json::parse(response);
        return file;
    }
    else {
        json file = json::parse(UNEXPECTED_RESPONSE);
        return file;
    }
}

json Client::readRetrieve(Strings response) {

    if (response[0]!=0) {
        json file = json::parse(response);
        return file;
    }
    else {
        json file = json::parse(UNEXPECTED_RETRIEVE);
        return file;
    }

}

json Client::jsonSignUp(Strings username, Strings password, Strings fullname) {
    json msg;
    msg["usrnm"] = username;
    msg["passw"] = sha256(password);
    msg["fullnm"] = fullname;
    return msg;
}
json Client::jsonLogin(Strings username, Strings password) {
    json msg;
    msg["usrnm"] = username;    
    msg["passw"] = sha256(password);
    return msg;
}

json Client::jsonChangePass(Strings newpass) {
    json msg;
    msg["oldpwd"] = my_hashpass;
    msg["newpwd"] = sha256(newpass);
    return msg;
}
json Client::jsonChangeColor(int color) {
    json msg;
    msg["color"] = color;
    return msg;
}
json Client::jsonChangeFullname(Strings fullname) {
    json msg;
    msg["fullnm"] = fullname;
    return msg;
}
json Client::jsonUserMsgs(Strings username) {
    json msg;
    msg["usrnm"] = username;
    return msg;
}
json Client::jsonRoomMsgs(RoomID roomID) {
    json msg;
    msg["room_id"] = roomID;
    return msg;
}

json Client::jsonSendMessage(Strings message, RoomID roomID) {
    json msg;
    msg["text"]  = message;
    msg["room_id"] = roomID;
    return msg;
}

void Client::findRoomName(json list, RoomID roomID, char* roomname) {
    rstString(roomname, ROOM_NAME_LEN);
    for (auto &user : list["info"])
    {
        if (roomID==user["roomid"].get<int>())
        {
            cpyString(roomname, ROOM_NAME_LEN, user["rname"].get<std::string>().c_str());
            return;
        }
    }
    if (roomname[0]==0) {
        roomname[0] = NOT_FOUND;
        return;
    }
}
void Client::findUserName(json list, Strings username, char* roomname) {
    rstString(roomname, ROOM_NAME_LEN);
    for (auto &user : list["info"])
    {
        if (cmpString(username, user["usrnm"].get<std::string>().c_str())==true)
        {
            cpyString(roomname, ROOM_NAME_LEN, user["fullnm"].get<std::string>().c_str());
            return;
        }
    }
    if (roomname[0]==0) {
        roomname[0] = NOT_FOUND;
        return;
    }
}

void Client::doFunction(int &page, int &option, int &event) {

    // PAGE = 1 - START MENU
    if (page==1) {
        // OPTION = 1: SIGN UP
        if (option==1) {

            rstString(username, USER_NAME_LEN);
            rstString(password, PASS_WORD_LEN);
            rstString(fullname, FULL_NAME_LEN);

            signUp(username, password, fullname);

            if (username[0]==ESC || password[0]==ESC || fullname[0]==ESC ) {
                event = ESC;
                return;
            }

            if (username[0]==EXIT || password[0]==EXIT || fullname[0]==EXIT) {
                event = EXIT;
                return;
            }

            request = jsonSignUp(username, password, fullname);

            std::cout << SAVE_CURSOR;

            sendRequest(SIGN_UP, request);
            response = readResponse(responseData);

            std::cout << RESTORE_CURSOR << CLEAR_AFTER;
            
            rstString(msg, MESSAGE_LEN);
            cpyString(msg, MESSAGE_LEN, response["msg"].get<std::string>().c_str());

            if (cmpString(msg, "OK")==true) {
                printFormat("\n\t<OK> Sign up successful!\n", GREEN, BOLD, LEFT, 0);
                printCountdown("Move to LOGIN PAGE", COUNTDOWN_SEC);
                option = 2;
            }
            else if (cmpString(msg, "ERROR")==true){
                printFormat("\n\t<ERRROR> ", RED, BOLD, LEFT, 0);
                printFormat(msg, RED, BOLD, LEFT, 0);
                std::cout << std::endl;
                printCountdown("Try to SIGN UP again", COUNTDOWN_SEC);
            }
            else {
                printFormat("\n\t<UNEXP> Unexpected receiving!\n", YELLOW, BOLD, LEFT, 0);
                printCountdown("Try to SIGN UP again", COUNTDOWN_SEC);
            }
        }
        // OPTION = 2: LOGIN
        else if (option==2) {

            rstString(my_username, USER_NAME_LEN);
            rstString(my_hashpass, HASH_PASS_LEN);

            rstString(username, USER_NAME_LEN);
            rstString(password, PASS_WORD_LEN);

            login(username, password);

            if (username[0]==ESC || password[0]==ESC) {
                event = ESC;
                return;
            }

            if (username[0]==EXIT || password[0]==EXIT) {
                event = EXIT;
                return;
            }

            request = jsonLogin(username, password);
            cpyString(my_hashpass, HASH_PASS_LEN, request["passw"].get<std::string>().c_str());

            std::cout << SAVE_CURSOR;

            sendRequest(LOGIN, request);
            response = readResponse(responseData);

            std::cout << RESTORE_CURSOR << CLEAR_AFTER;
            
            rstString(msg, MESSAGE_LEN);
            cpyString(msg, MESSAGE_LEN, response["msg"].get<std::string>().c_str());
            rstString(info, MESSAGE_LEN);
            cpyString(info, MESSAGE_LEN, response["info"].dump().c_str());
            if (cmpString(msg, "OK")==true) {
                printFormat("\n\t<OK> Login successful!\n", GREEN, BOLD, LEFT, 0);
                cpyString(my_fullname, FULL_NAME_LEN, response["info"]["fullnm"].get<std::string>().c_str());
                cpyString(my_username, USER_NAME_LEN, response["info"]["usrnm"].get<std::string>().c_str());
                my_color = response["info"]["color"].get<int>();
                printCountdown("Move to MAIN MENU", COUNTDOWN_SEC);
                option = 0;
                page = 2;
            }
            else if (cmpString(msg, "ERROR")==true){
                rstString(my_hashpass, HASH_PASS_LEN);
                printFormat(msg, RED, BOLD, LEFT, 0);
                std::cout << std::endl;
                printCountdown("Try to SIGN UP again", COUNTDOWN_SEC);
            }
            else {
                printFormat("\n\t<UNEXP> Unexpected receiving!\n", YELLOW, BOLD, LEFT, 0);
                printCountdown("Try to SIGN UP again", COUNTDOWN_SEC);
            }
        }
        // OPTION INVALID
        else {
            event = INVALID;
            return;
        }
    }
    // PAGE = 2 - FULL ACCESSING MENU
    else { // if (page==2)
        // OPTION = 1: CHATTING OPTION MENU
        if (option==1) {
            std::cout << "Your option: ";
            while (true) {  // CHOOSE CHAT MODE
                std::cout << SAVE_CURSOR;
                std::cout << CLEAR_AFTER_INLINE;
                chatting_mode = cinChar(DIGIT);
                if (chatting_mode==ESC) {
                    event = ESC;
                    return;
                }
                else if (chatting_mode==EXIT) {
                    event = EXIT;
                    return;
                }
                else if (chatting_mode==INVALID) {
                    printFormat("\t<!> Unavailable option!\n", YELLOW, ITALIC, LEFT, 0);
                    std::cout << RESTORE_CURSOR;
                    continue;
                }
                else {
                    break;
                }
            }
            request = emptyRequest();

            std::cout << SAVE_CURSOR;

            if (chatting_mode==1) {
                sendRequest(FIND_USER, request);
            }
            else if (chatting_mode==2) {
                sendRequest(FIND_ROOM, request);
            }

            std::cout << RESTORE_CURSOR << CLEAR_AFTER;

            std::cout << CLEAR_ALL;
            printMenu(page, 10+chatting_mode, getConsoleLength());

            response = readResponse(responseData);

            if (chatting_mode==1) { // LIST ALL USERS

                printAllUser(response, my_username);

                printFormats("\t >> Enter USERNAME that you want to chat: ", CYAN, BOLD, ITALIC, LEFT, 0);

                while (true) { // type user name

                    std::cout << SAVE_CURSOR;

                    rstString(username, USER_NAME_LEN);
                    cinString(username, USER_NAME_LEN, UNHIDE);

                    if (username[0]==ESC) {
                        event = ESC;
                        return;
                    }

                    else if (username[0]==EXIT) {
                        event = EXIT;
                        return;
                    }

                    if (cmpString(username, my_username)==true) {
                        printFormat("\t<!> This app NOT allowed you to message yourself!\n", YELLOW, ITALIC, LEFT, 0);
                        std::cout << RESTORE_CURSOR << CLEAR_AFTER_INLINE;
                        rstString(username, USER_NAME_LEN);
                        continue;
                    }
                    std::cout << CLEAR_AFTER;

                    findUserName(response, username, my_roomname);

                    if (my_roomname[0]==NOT_FOUND) {
                        printFormat("\t<!> Username not found!\n", YELLOW, ITALIC, LEFT, 0);
                        std::cout << RESTORE_CURSOR << CLEAR_AFTER_INLINE;
                        rstString(username, USER_NAME_LEN);
                        continue;
                    }

                    else {
                        break;
                    }

                }

                std::cout << SAVE_CURSOR;

                request = jsonUserMsgs(username);
                sendRequest(REMSGS_USER, request);

                std::cout << RESTORE_CURSOR << CLEAR_AFTER;
            }

            else if (chatting_mode==2) { // LIST ALL PUBLIC ROOMS

                printAllRoom(response);

                printFormats("\t >> Enter ROOM ID that you want to join: ", CYAN, BOLD, ITALIC, LEFT, 0);

                while (true) {  // type room id
                    
                    std::cout << SAVE_CURSOR;

                    room_id = cinNumber();

                    if (room_id==ESC) {
                        event = ESC;
                        return;
                    }
                    if (room_id==EXIT) {
                        event = EXIT;
                        return;
                    }
                    if (room_id==INVALID) {
                        event = INVALID;
                        return;
                    }

                    findRoomName(response, room_id, my_roomname);

                    if (my_roomname[0]==NOT_FOUND) {
                        printFormat("\t<!> Room ID not found!\n", YELLOW, ITALIC, LEFT, 0);
                        std::cout << RESTORE_CURSOR;
                        std::cout << CLEAR_AFTER;
                        continue;
                    }

                    else {
                        break;
                    }

                }

                std::cout << SAVE_CURSOR;

                request = jsonRoomMsgs(room_id);
                sendRequest(REMSGS_ROOM, request);

                std::cout << RESTORE_CURSOR << CLEAR_AFTER;
            }

            retrieve = readRetrieve(responseData);
            printCountdown("Move to CHAT BOX CONSOLE", COUNTDOWN_SEC);
            page = 3;
            option = chatting_mode;
        }
        // OPTION = 2: SETTING
        else if (option==2) {

            std::cout << "Your option: ";
            while (true) {  // SETTING OPTION MENU
                std::cout << SAVE_CURSOR;
                std::cout << CLEAR_AFTER_INLINE;
                setting_mode = cinChar(DIGIT);
                if (setting_mode==ESC) {
                    event = ESC;
                    return;
                }
                else if (setting_mode==EXIT) {
                    event = EXIT;
                    return;
                }
                else if (setting_mode==INVALID) {
                    printFormat("\t<!> Unavailable option!\n", YELLOW, ITALIC, LEFT, 0);
                    std::cout << RESTORE_CURSOR;
                    continue;
                }
                else {
                    break;
                }
            }

            if (setting_mode==1) {          // CHANGE PASSWORD
                rstString(password, PASS_WORD_LEN);
                changePassword(password, my_hashpass, my_username);
                if (password[0]==ESC) {
                    event = ESC;
                    return;
                }
                if (password[0]==EXIT) {
                    event = EXIT;
                    return;
                }
            }
            else if (setting_mode==2) {     // CHANGE COLOR
                changeColor(color);
                if (color==ESC) {
                    event = ESC;
                    return;
                }
                if (color==EXIT) {
                    event = EXIT;
                    return;
                }
                if (color==INVALID) {
                    event = INVALID;
                    return;
                }
                std::cout << std::endl;
                std::cout << TAB << "- Current  : " << colorName(my_color) << std::endl;
                std::cout << TAB << "- Change to: " << colorName(color) << std::endl;
            }
            else if (setting_mode==3) {     // CHANGE FULLNALE
                rstString(fullname, FULL_NAME_LEN);
                changeFullname(fullname);
                if (fullname[0]==ESC) {
                    event = ESC;
                    return;
                }
                if (fullname[0]==EXIT) {
                    event = EXIT;
                    return;
                }
                std::cout << std::endl;
                std::cout << TAB << "- Current name:  " << my_fullname << std::endl;
                std::cout << TAB << "- The new name:  " << fullname << std::endl;
            }

            // END OF TYPING
            confirmChange(confirm);
            if (confirm==1) {
  
                std::cout << SAVE_CURSOR;

                if (setting_mode==1) {
                    request = jsonChangePass(password);
                    sendRequest(CHANGE_PASSWD, request);
                }
                else if (setting_mode==2) {
                    request = jsonChangeColor(color);
                    sendRequest(CHANGE_COLORS, request);
                }
                else if (setting_mode==3) {
                    request = jsonChangeFullname(fullname);
                    sendRequest(CHANGE_FULLNM, request);
                }

                std::cout << RESTORE_CURSOR << CLEAR_AFTER;
                response = readResponse(responseData);

                rstString(msg, MESSAGE_LEN);
                cpyString(msg, MESSAGE_LEN, response["msg"].get<std::string>().c_str());

                if (cmpString(msg, "OK")==true) {
                    if (setting_mode==1) {
                        printFormat("\n\t<OK> Change password successful!\n", GREEN, BOLD, LEFT, 0);
                        cpyString(my_hashpass, HASH_PASS_LEN, response["passw"].get<std::string>().c_str());
                    }
                    else if (setting_mode==2) {
                        printFormat("\n\t<OK> Change color successful!\n", GREEN, BOLD, LEFT, 0);
                        my_color = response["info"]["color"].get<int>();
                    }
                    else if (setting_mode==3) {
                        printFormat("\n\t<OK> Change fullname successful!\n", GREEN, BOLD, LEFT, 0);
                        cpyString(my_fullname, FULL_NAME_LEN, response["info"]["fullnm"].get<std::string>().c_str());
                    }
                }
                else {
                    printFormat("\n\t<ERRROR> ", RED, BOLD, LEFT, 0);
                    printFormat(msg, RED, BOLD, LEFT, 0);
                    std::cout << std::endl;
                }
            }
            else if (confirm==0) {
                return;
            }
            else if (confirm==EXIT) {
                event = EXIT;
                option = 0;
                page = 0;
                return;
            }

            printCountdown("Move to MAIN MENU", COUNTDOWN_SEC);
            option = 0;
            page = 2;
        }
        // OPTION = 3: GET INFORMATION
        else if (option==3) {
            std::cout << "Your option: ";
            while (true) {  // SETTING OPTION MENU
                std::cout << SAVE_CURSOR;
                std::cout << CLEAR_AFTER_INLINE;
                helping_mode = cinChar(DIGIT);
                if (helping_mode==ESC) {
                    event = ESC;
                    return;
                }
                else if (helping_mode==EXIT) {
                    event = EXIT;
                    return;
                }
                else if (helping_mode==INVALID) {
                    printFormat("\t<!> Unavailable option!\n", YELLOW, ITALIC, LEFT, 0);
                    std::cout << RESTORE_CURSOR;
                    continue;
                }
                else {
                    break;
                }
            }

            if (helping_mode==1) {          // GET PERSONAL INFORMATION
                std::cout << "\n\t- Your fullname: ";
                printFormat(my_fullname, DEFAULT, ITALIC, LEFT, 0);
                std::cout << "\n\t- Your username: ";
                printFormats(my_username, DEFAULT, BOLD, ITALIC,  LEFT, 0);
                std::cout << "\n\t- Your color: " << colorName(my_color);
            }
            else if (setting_mode==2) {     // GET APPLICATION INFORMATION
                request = json::parse(EMPTY_REQUEST);

                std::cout << SAVE_CURSOR;

                sendRequest(GET_INFO, request);
                response = readResponse(responseData);

                std::cout << RESTORE_CURSOR << CLEAR_AFTER;

                printAppInfo(response);
            }
            printFormats("Press ESC to return the previous page", YELLOW, BOLD, ITALIC, CENTER, getConsoleLength());
            // WAIT FOR USER PRESS ESC
            int ch=0;
            while(true)
            {
                ch = cinChar(DIGIT);
                if (ch==ESC) {
                    break;
                }
                else if (ch==EXIT) {
                    event = EXIT;
                    return;
                }
                else {
                    continue;
                }
            }

            printCountdown("Move to MAIN MENU", COUNTDOWN_SEC);
            option = 0;
            page = 2;
        }
        // OPTION INVALID
        else {
            event = INVALID;
            return;
        }
    }
    request = emptyRequest();
}

void Client::resetAttribute() {
    rstString(my_hashpass, HASH_PASS_LEN);
    rstString(my_username, USER_NAME_LEN);
    rstString(my_roomname, USER_NAME_LEN);
    rstString(requestData, REQUEST_LEN);
    rstString(responseData, RESPONSE_LEN);

    my_color = DEFAULT;
    retrieve = emptyRequest();
}

bool Client::isLoggedIn() {
    if (my_hashpass[0]==0) {
        return false;
    }
    return true;
}

Client::~Client() {

}

void printAllUser(json list, Strings yourUsername) {
    int size = list["info"].size();
    int lenconsole = getConsoleLength();
    int hghconsole = getConsoleHeight();
    bool divide = (size > (hghconsole-6)) && ((USER_NAME_LEN+1+FULL_NAME_LEN+2) <= lenconsole);
    
    int index=0;
    int space = lenconsole/2 - (USER_NAME_LEN+1+FULL_NAME_LEN+2);

    for (auto &user : list["info"]) {
        usleep(50000);
        rstString(username, USER_NAME_LEN);
        cpyString(username, USER_NAME_LEN, user["usrnm"].get<std::string>().c_str());
        rstString(fullname, FULL_NAME_LEN);
        cpyString(fullname, FULL_NAME_LEN, user["fullnm"].get<std::string>().c_str());
        if (cmpString(yourUsername, username)==false) {
            std::cout << "\033[" << space/2 << "C";
            std::cout << "\033[" << USER_NAME_LEN-lenString(username)+1 << "C";

            std::cout << "\033[" << ITALIC << "m";
            std::cout << '@' << username;
            std::cout << DEFAULT_FORMAT;

            std::cout << "\033[2C";

            std::cout << "\033[" << user["color"].get<int>() << "m";
            std::cout << "\033[" << BOLD << "m";
            std::cout << fullname;
            std::cout << DEFAULT_FORMAT;

            std::cout << "\033[" << FULL_NAME_LEN-lenString(fullname) << "C";
            std::cout << "\033[" << space/2 << "C";

            if (index%2==0) {
                if (divide==true) {
                    std::cout << ".";
                }
                else {
                    std::cout << std::endl;
                }
            }
            else {
                std::cout << std::endl;
            }
        }
        index++;
    }
}

void printAllRoom(json list) {
    int size = list["info"].size();
    int lenconsole = getConsoleLength();
    int hghconsole = getConsoleHeight();
    bool divide = (size > (hghconsole-6)) && ((NUMBER_LEN+5+ROOM_NAME_LEN+3) <= lenconsole);
    
    int index=0;
    int space = lenconsole/2 - (NUMBER_LEN+5+ROOM_NAME_LEN+3);
    for (auto &user : list["info"]) {
        usleep(50000);
        room_id = user["roomid"].get<int>();
        rstString(roomname, ROOM_NAME_LEN);
        cpyString(roomname, ROOM_NAME_LEN, user["rname"].get<std::string>().c_str());
            std::cout << "\033[" << space/2 << "C";
            std::cout << "\033[" << NUMBER_LEN-numDigits(room_id)+5 << "C";

            std::cout << "\033[" << BOLD << "m";
            std::cout << "(ID) ";
            std::cout << room_id;
            std::cout << DEFAULT_FORMAT;

            std::cout << " - ";

            std::cout << "\033[" << CYAN << "m";
            std::cout << roomname;
            std::cout << DEFAULT_FORMAT;

            std::cout << "\033[" << ROOM_NAME_LEN-lenString(roomname) << "C";
            std::cout << "\033[" << space/2 << "C";

            if (index%2==0) {
                if (divide==true) {
                    std::cout << ".";
                }
                else {
                    std::cout << std::endl;
                }
            }
            else {
                std::cout << std::endl;
            }
        index++;
    }
}

void printAppInfo(json list) {
    for (auto &info : list["info"]) {
        rstString(msg, MESSAGE_LEN);
        cpyString(msg, MESSAGE_LEN, info["text"].get<std::string>().c_str());
        std::cout << TAB;
        printFormat(msg, CYAN, ITALIC, LEFT, 0);
        std::cout << std::endl;
    }
}

void printMessage(Strings username, Strings message, Color color, int range, Strings yourUsername, Strings preUsername) {
    if (cmpString(username, yourUsername)==true) {
        // ALIGNS ON THE RIGHT, display username
        if (cmpString(username, preUsername)==false) {
            printFormats("You", DEFAULT, LIGHT, ITALIC, RIGHT, range-5);
            std::cout << std::endl;
        }
        printFormat(message, color, BOLD, RIGHT, range-5);
        std::cout << std::endl;
    }
    else {
        // ALIGNS ON THE LEFT, display username
        if (cmpString(username, preUsername)==false) {
            printFormats(username, DEFAULT, LIGHT, ITALIC, LEFT, range);
            std::cout << std::endl;
        }
        printFormat(message, color, BOLD, LEFT, range);
        std::cout << std::endl;
    }
}

void printAllMsgs(json list, Strings yourUsername) {

    int height=0, length=0;
    getConsoleSize(height, length);

    if (list["info"]["history"].size() == 0) {
        printFormat("Say hello to your friend!", DEFAULT, ITALIC, CENTER, length);
        std::cout << std::endl;
        return;
    }

    rstString(preusrnm, USER_NAME_LEN);

    for (auto &msgs : list["info"]["history"]) {
        
        rstString(username, USER_NAME_LEN);
        cpyString(username, USER_NAME_LEN, msgs["usrnm"].get<std::string>().c_str());

        rstString(msg, MESSAGE_LEN);
        cpyString(msg, MESSAGE_LEN, msgs["text"].get<std::string>().c_str());

        printMessage(username, msg, msgs["color"].get<int>(), length, yourUsername, preusrnm);

        rstString(preusrnm, USER_NAME_LEN);
        cpyString(preusrnm, USER_NAME_LEN, username);
    }
}

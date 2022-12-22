#include "../include/client.h"

char message[MESSAGE_LEN+1];

void recvMessage(Client *client, json file) {
    json retrieve;
    int newest=0;
    int size=0;
    while (client->chatting)
    {
        if (client->sending) {
            continue;
        }
        else {
            client->sendRequest(REMSGS_ROOM, file);
            retrieve = client->readRetrieve(client->responseData);
            size = retrieve["info"]["history"].size();
            if (size > 0) {
                if (newest != retrieve["info"]["history"][size]["id"].get<int>()) {
                    client->retrieve = retrieve;
                    client->newmsg = true;
                    newest = retrieve["info"]["history"][size]["id"].get<int>();
                }
                else {
                    client->newmsg = false;
                }
            }
        }
        usleep(500000);
    }
}

int height = 0;
int length = 0;

Client client;

int main() {
    int check=0;
    while (true) {
        client.createSocket();
        check = client.connectServer();
        if (check != -1) {
            break;
        }
    }

    int page = START_APP;
    int option = NONE;

    while (page != EXIT_APP) {

        getConsoleSize(height, length);
        printMenu(page, option, length);

        if (option==NONE) {  // option has not been chosen yet

            std::cout << "Your option = ";
            option = cinChar(DIGIT);

            if (option==ESC) {
                if (page==HOME_PAGE) {

                    int confirm = logout();
                    if (confirm==1) {

                        rstString(client.requestData, REQUEST_LEN);
                        cmpString(client.requestData, EMPTY_REQUEST);

                        client.sendRequest(LOGOUT, client.requestData);
                        client.resetAttribute();
                    }
                    else if (confirm==0) {
                        option = 0;
                        continue;
                    }
                    else if (confirm==EXIT) {

                        rstString(client.requestData, REQUEST_LEN);
                        cmpString(client.requestData, EMPTY_REQUEST);

                        client.sendRequest(LOGOUT, client.requestData);
                        client.resetAttribute();

                        page = 0;
                        option = 0;
                        continue;
                    }
                }

                page = page - 1;
                option = NONE;
                continue;
                std::cout << "Prepare to logout";
            }
            else if (option==EXIT) {

                rstString(client.requestData, REQUEST_LEN);
                cmpString(client.requestData, EMPTY_REQUEST);

                client.sendRequest(LOGOUT, client.requestData);
                client.resetAttribute();

                page = 0;
                option = 0;
                continue;
            }
            else {
                continue;
            }
        }
        else {
            int event = 0;
            int backup_option = option;

            if (page != CHAT_PAGE) {
                client.doFunction(page, backup_option, event);
            }
            else { // REALTIME CHAT

                // PREPARE TO CHATTING
                json request;
                json response;

                RoomID room_id;
                room_id  = client.retrieve["info"]["roomid"];
                request = client.jsonRoomMsgs(room_id);

                client.chatting = true;
                std::thread RECV(recvMessage, &client, request);

                int len = 0;
                char ch = 0;

                rstString(message, MESSAGE_LEN);
                request = client.emptyRequest();
                system("stty -icanon min 0 time 20");

                // JOIN INTO REALTIME CHATTING
                int receiveCheck;
                client.newmsg = true;
                while (event==0) {
                    if (client.newmsg) {
                        getConsoleSize(height, length);
                        printMenu(page, option, length);
                        std::cout << std::endl;

                        std::cout << SAVE_CURSOR;

                        std::cout << "\033[" << 4 << ";" << (length-lenString(client.my_roomname))/2 << "H";
                        std::cout << "\033[" << CYAN << "m";
                        std::cout << "\033[" << BOLD << "m";
                        std::cout << "\033[" << ITALIC << "m";
                        std::cout << client.my_roomname;

                        std::cout << RESTORE_CURSOR;

                        printAllMsgs(client.retrieve, client.my_username);
                        std::cout << "\033[" << height-2 << ";1H";
                        std::cout << "-----------------------------------------" << std::endl;
                        std::cout << "Your message: " << message << std::flush;
                    }
                    ch = 0;
                    while (read(0, &ch, 1) == 1) {
                        std::cout << "\033[" << DEFAULT << "m";
                        if (ch==27) {
                            event = ESC;
                            break;
                        }
                        if (ch==3 || ch==26) {
                            event = EXIT;
                            break;
                        }
                        else if (ch==127) {
                            message[--len] = 0;
                            std::cout << BACK_SIGNAL << std::flush;
                            if (len>0) {
                                std::cout << BACK_SPACE << std::flush;
                            }
                        }
                        else if (ch=='\n' && len>0) {
                            std::cout << BACK_LINE << std::flush;
                            std::cout << CLEAR_AFTER << std::flush;
                            std::cout << ".............................Sending............................" << std::flush;
                            
                            if (message[0]=='$') {  // COMMAND MESSAGE
                                // CHANGE COLOR
                                if (findString(message, "color") > 0) {
                                    int color = getArgumentColor(message);
                                    request = client.jsonChangeColor(color);
                                    client.sendRequest(CHANGE_COLORS, request);
                                    response = client.readResponse(client.responseData);
                                    if (cmpString(response["msg"].get<std::string>().c_str(), "OK")==true)
                                    {
                                        client.my_color = color;
                                        client.newmsg = true;
                                    }
                                }
                                // else ...
                            }
                            else {  // CONTENT MESSAGE
                                request = client.jsonSendMessage(message, room_id);
                                client.sendRequest(SEND_MSG, request);
                                response = client.readResponse(client.responseData);
                                if (cmpString(response["msg"].get<std::string>().c_str(), "OK")==true)
                                {
                                    std::cout << HOME_LINE << CLEAR_AFTER_INLINE;
                                    rstString(message, MESSAGE_LEN);
                                    len = 0;
                                }
                                else {
                                    std::cout << "\033[" << RED << "m";
                                }
                            }
                            request = client.emptyRequest();
                        }
                        else {
                            message[len++] = ch;
                        }
                    }
                }
                std::cout << BACK_SIGNAL << std::flush;

                // OUT OF REALTIME CHATTING
                client.chatting = false;
                RECV.join();

                std::cout << "\033[" << height-2 << ";1H";
                std::cout << CLEAR_AFTER;
                std::cout << "-----------------------------------------" << std::endl;
                std::cout << "<NOTE> Press ESC key to return ";
                while(ch=cinChar(ANYCH)) {
                    if (ch==ESC) {
                        break;
                    }
                }
                printCountdown("Return to MAIN MENU", COUNTDOWN_SEC);
                page = 2;
                
            }

            if (event==ESC) {
                option = 0;
                continue;
            }
            else if (event==INVALID) {
                option = 0;
                continue;
            }
            else if (event==EXIT) {
                rstString(client.requestData, REQUEST_LEN);
                cpyString(client.requestData, REQUEST_LEN, EMPTY_REQUEST);

                client.sendRequest(LOGOUT, client.requestData);
                client.resetAttribute();
                page = 0;
                option = 0;
                continue;
            }
            else {
                option = backup_option;
            }
        }
    }

    goodByeApp(length);   
    return 0;
}

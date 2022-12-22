#include "../include/io.h"

char hashpass[HASH_PASS_LEN+1];
char repasswd[PASS_WORD_LEN+1];
char buffer[10+1];

void printFrame(int borderline, Strings message, int range) {
    int i=0;
    if (borderline==FIRST_LINE) {
        std::cout << std::endl;
        std::cout << "\033[" << BOLD << "m";
        std::cout << "\033[" << YELLOW << "m";
        std::cout << "  +";
        int limit = (range)/2 - lenString(message)/2;
        for (i=3; i<limit; i++) {
            std::cout << "-";
        }
        std::cout << message;
        for (i+=lenString(message); i<range-3; i++) {
            std::cout << "-";
        }
        std::cout << "+";
    }
    else if (borderline==BODY_LINE) {
        std::cout << "\033[" << BOLD << "m";
        std::cout << "\033[" << YELLOW << "m";
        std::cout << "  |";
        int limit = (range)/2 - lenString(message)/2;
        for (i=3; i<limit; i++) {
            std::cout << " ";
        }
        std::cout << "\033[" << GREEN << "m";
        std::cout << message;
        for (i+=lenString(message); i<range-3; i++) {
            std::cout << " ";
        }
        std::cout << "\033[" << YELLOW << "m";
        std::cout << "|"; 
    }
    else if (borderline==LAST_LINE) {
        std::cout << "\033[" << BOLD << "m";
        std::cout << "\033[" << YELLOW << "m";
        std::cout << "  +";
        for (i=3; i<range-3; i++) {
            std::cout << "-";
        }
        std::cout << "+";
    }
    else { // if (borderline==NOTIVE_LINE) {
        std::cout << "\033[" << BOLD << "m";
        std::cout << "\033[" << YELLOW << "m";
        std::cout << "  |";
        std::cout << "\033[1B" << "\033[1D" << "|";
        int limit = (range)/2 - lenString(message)/2;
        for (i=3; i<limit; i++) {
            std::cout << " ";
        }
        std::cout << "\033[" << REGULAR << "m";
        std::cout << "\033[" << ITALIC << "m";
        std::cout << "\033[" << RED << "m";
        std::cout << message;
        for (i+=lenString(message); i<range-3; i++) {
            std::cout << " ";
        }
        std::cout << "\033[" << REGULAR << "m";
        std::cout << "\033[" << BOLD << "m";
        std::cout << "\033[" << YELLOW << "m";
        std::cout << "|";
        std::cout << "\033[1A" << "\033[1D" << "|" << "\033[1B";
    }
    std::cout << DEFAULT_FORMAT;
    std::cout << std::endl;
}
void printMenu(int page, int option, int length) {
    std::cout << CLEAR_ALL;
    // PAGE = 1
    if (page==1) {
        // OPTION = 0
        if (option==0) {
            printFrame(FIRST_LINE, " PAGE 1: START MENU ", length);

            printFrame(BODY_LINE, "[1] - CREAT NEW ACCOUNT", length);
            printFrame(BODY_LINE, "[2] - LOGIN            ", length);

            printFrame(NOTICE_LINE, "Press ESC for exit the app", length);

            printFrame(LAST_LINE, "", length);
        }
        // OPTION = 1
        if (option==1) {
            printFrame(FIRST_LINE, " PAGE 1-1: CREAT NEW ACCOUNT ", length);

            printFrame(BODY_LINE, "Please follow the instructions", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
        // OPTION = 2
        if (option==2) {
            printFrame(FIRST_LINE, " PAGE 1-2: LOGIN ", length);

            printFrame(BODY_LINE, "Please follow the instructions", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
    }
    // PAGE = 2
    if (page==2) {
        // OPTION = 0
        if (option==0)
        {
            printFrame(FIRST_LINE, " PAGE 2: MAIN MENU ", length);

            printFrame(BODY_LINE, "[1] - CHAT       ", length);
            printFrame(BODY_LINE, "[2] - SETTING    ", length);
            printFrame(BODY_LINE, "[3] - INFORMATION", length);

            printFrame(NOTICE_LINE, "Press ESC for logout", length);

            printFrame(LAST_LINE, "", length);
        }
        // OPTION = 1
        if (option==1)
        {
            printFrame(FIRST_LINE, " PAGE 2-1: CHAT MENU ", length);

            printFrame(BODY_LINE, "[1] - CHAT WITH ANOTHER USER", length);
            printFrame(BODY_LINE, "[2] - CHAT ON A PUBLIC ROOM ", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
        // OPTION = 11
        if (option==11)
        {
            printFrame(FIRST_LINE, " LIST OF USERS ", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
        // OPTION = 11
        if (option==12)
        {
            printFrame(FIRST_LINE, " LIST OF ROOMS ", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
    // OPTION = 2
        if (option==2)
        {
            printFrame(FIRST_LINE, " PAGE 2-2: SETTING ", length);

            printFrame(BODY_LINE, "[1] - CHANGE YOUR PASSWORD ", length);
            printFrame(BODY_LINE, "[2] - CHANGE YOUR COLOR    ", length);
            printFrame(BODY_LINE, "[3] - CHANGE YOUR FULL NAME", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
    // OPTION = 3
        if (option==3)
        {
            printFrame(FIRST_LINE, " PAGE 2-3: INFORMATION ", length);

            printFrame(BODY_LINE, "[1] - MY PERSONAL INFORMATION", length);
            printFrame(BODY_LINE, "[2] - ABOUT THIS APPLICATION ", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
    }
    // PAGE = 3
    if (page==3) {
        // OPTION = 1
        if (option==1) {
            printFrame(FIRST_LINE, " PAGE 3: CHAT SONSOLE ", length);
            
            printFrame(BODY_LINE, "CHAT WITH USER", length);
            printFrame(BODY_LINE, "", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
        // OPTION = 2
        if (option==2) {
            printFrame(FIRST_LINE, " PAGE 3: CHAT SONSOLE ", length);
            
            printFrame(BODY_LINE, "CHAT ON ROOM", length);
            printFrame(BODY_LINE, "", length);

            printFrame(NOTICE_LINE, "Press ESC for back to previous page", length);

            printFrame(LAST_LINE, "", length);
        }
    }
}

Strings colorName(int color) {
    switch (color) {
    case BLACK:
        return "\033[30mBLACK\033[39m";
    case RED:
        return "\033[31mRED\033[39m";
    case GREEN:
        return "\033[32mGREEN\033[39m";
    case YELLOW:
        return "\033[33mYELLOW\033[39m";
    case BLUE:
        return "\033[34mBLUE\033[39m";
    case MAGENTA:
        return "\033[35mMAGENTA\033[39m";
    case CYAN:
        return "\033[36mCYAN\033[39m";
    case WHITE:
        return "\033[37mWHITE\033[39m";
    default:
        return "\033[38mDEFAULT\033[39m";
    }
}
void printListColor() {
    std::cout << std::endl;
    std::cout << TAB << "List color:  ";
    for (int color=1; color<8; color++)
    {
        std::cout << "\033[" << color+40 << "m";
        std::cout << " [" << color << "] ";
        std::cout << "\033[0m";
        std::cout << "  ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void printFormats(Strings message, Color color, Format format, Format format2, Align align=LEFT, int range=0) {
    std::cout << "\033[" << format2 << "m";
    std::cout << "\033[" << format  << "m";
    std::cout << "\033[" << color   << "m";
    if (align==LEFT) {
        std::cout << std::setw(range) << std::left << message;
    }
    else if (align==RIGHT) {
        std::cout << std::setw(range) << std::right << message;
    }
    else if (align==CENTER) {
        int len = lenString(message);
        std::cout << "\033[" << (range/2-len/2) << "C";
        std::cout << message;
    }
    std::cout << DEFAULT_FORMAT;
}
void printFormat(Strings message, Color color, Format format, Align align=LEFT, int range=0) {
    std::cout << "\033[" << format  << "m";
    std::cout << "\033[" << color   << "m";
    if (align==LEFT) {
        std::cout << std::setw(range) << std::left << message;
    }
    else if (align==RIGHT) {
        std::cout << std::setw(range) << std::right << message;
    }
    else if (align==CENTER) {
        int len = lenString(message);
        std::cout << "\033[" << (range/2-len/2) << "C";
        std::cout << message;
        std::cout << "\033[" << (range/2-len/2) << "C";
    }
    std::cout << DEFAULT_FORMAT;
}
// print message in x seconds
void printCountdown(Strings message, int second=COUNTDOWN_SEC) {
    while (second) {
        std::cout << HOME;
        std::cout << message;
        std::cout << " in " << second << " seconds ..." << std::flush;
        second--;
        sleep(1);
    }
}

// type: DIGIT, YESNO, COLOR or ANYCH
int cinChar(int type) {
    char ch = 0;
    while (true)
    {
        system("stty raw");
        ch = getchar();
        system("stty -raw");
        if (ch==10 || ch==13) {  // ENTER
            std::cout << BACK_SIGNAL;
            continue;
        }
        else
        {   
            if (ch<32) {    // INVISIBLE ASCII CHARACTER
                std::cout << BACK_SIGNAL;
                if (ch==3) {        // Ctrl-C
                    std::cout << "Ctrl-C";
                    std::cout << std::endl;
                    return EXIT;
                }
                else if (ch==26) {  // Ctrl-Z
                    std::cout << "Ctrl-Z";
                    std::cout << std::endl;
                    return EXIT;
                }
                else if (ch==27) {  // ESC
                    std::cout << "ESC" << std::endl;
                    return ESC;
                }
                else {
                    continue;
                }
            }
            else {          //   VISIBLE ASCII CHARACTER
                if (type==DIGIT) {
                    if (ch >= '0' && ch <= '9') {
                        std::cout << std::endl;
                        return int(ch) - 48;
                    }
                    else if (ch==127) { // BACK_SPACE
                        std::cout << BACK_SIGNAL;
                        continue;
                    }
                    else {
                        std::cout << BACK_SPACE;
                        continue;                    
                    }
                }
                else if (type==YESNO) {
                    if (ch=='Y' || ch=='y' || ch=='N' || ch=='n') {
                        std::cout << std::endl;
                        return int(ch);
                    }
                    else if (ch==127) { // BACK_SPACE
                        std::cout << BACK_SIGNAL;
                        continue;
                    }
                    else {
                        std::cout << BACK_SPACE;
                        continue;                    
                    }
                }
                else if (type==COLOR) {
                    if (ch >= '0' && ch <= '9') {
                        std::cout << std::endl;
                        return int(ch) - 48 + 30;
                    }
                    else if (ch==127) { // BACK_SPACE
                        std::cout << BACK_SIGNAL;
                        continue;
                    }
                    else {
                        std::cout << BACK_SPACE;
                        continue;                    
                    }
                }
                else {
                    if (ch==127) { // BACK_SPACE
                        std::cout << BACK_SIGNAL;
                        continue;
                    }
                    else {
                        return int(ch);
                    }
                }
            }
        }
    }
}

int getValue(Strings number) {
    int index=0;
    int value=0;
    while (number[index]) {
        if (number[index]<48 || number[index]>57) {
            std::cout << "<ERR> Invalid number" << std::endl;
            return -1;
        }
        value = value*10 + (int(number[index])-48);
        index++;
    }
    return value;
}
int cinNumber() {
    char number[NUMBER_LEN+1] = {0};
    cinString(number, NUMBER_LEN, UNHIDE);
    if (number[0]==ESC) {
        return ESC;
    }
    return getValue(number);
}
int numDigits(int value) {
    int num=0;
    while (value>0) {
        value = value/10;
        num++;
    }
    return num;
}

// system("stty raw");     // Allows raw mode input
// system("stty -raw");    // Allows canonical input mode
int cinString(char *str, const int size, bool hiding=UNHIDE) {
    int lenStr = lenString(str);
    char ch = 0;
    while (lenStr<size) {

        system("stty raw");
        ch = getchar();
        system("stty -raw");

        if (ch<32) {    // INVISIBLE ASCII CHARACTER
            std::cout << BACK_SIGNAL;
            if (ch==3) {        // Ctrl-C
                std::cout << "Ctrl-C";
                str[0] = EXIT;
                break;
            }
            else if (ch==26) {  // Ctrl-Z
                std::cout << "Ctrl-Z";
                str[0] = EXIT;
                break;
            }
            else if (ch==27) {  // ESC
                std::cout << "ESC";
                str[0] = ESC;
                break;
            }
            else if (ch==13) {  // ENTER
                if (lenStr==0) {
                    continue;
                }
                else {
                    str[lenStr] = 0;
                    break;
                }
            }
            else if (ch==8) {
                if (lenStr>0) {
                    std::cout << BACK_SPACE;
                    lenStr = lenStr - 1;
                    str[lenStr] = 0;
                }
                else {
                    continue;
                }
            }
        }
        else {          //   VISIBLE ASCII CHARACTER
            if (ch==127) {     // BACK_SPACE
                std::cout << BACK_SIGNAL;
                if (lenStr>0) {
                    std::cout << BACK_SPACE;
                    lenStr = lenStr - 1;
                    str[lenStr] = 0;
                }
                else {
                    continue;
                }
            }
            else {              // FROM 32 (' ') TO 126 ('~')
                if (lenStr==size-1) {   // BUFFER IS FULL
                    std::cout << BACK_SPACE;
                }
                else {
                    str[lenStr] = ch;
                    lenStr = lenStr + 1;
                    if (hiding==true) {
                        std::cout << BACK_SPACE << char(42);
                    }
                    else {
                        continue;
                    }
                }
            }
        }
    }
    std::cout << std::endl;
    return lenStr;
}

int findString(Strings str_A, Strings str_B) {
    int a=0, b=0;
    int pos;

    while(str_A[a] && str_B[b]) {
        if (str_A[a]!=str_B[b]) {
            b=0;
        }
        else {
            if (b==0) {
                pos = a;
            }
            b++;
        }
        a++;
    }
    if (a-pos == b) {
        return pos;
    }
    return -1;
}
void rstString(char *str, const int size) {
    int i=0;
    while (i<size) {
        str[i++] = 0;
    }
}
bool cmpString(Strings str_A, Strings str_B) {
    int a=0;
    int b=0;
    while (str_A[a] && str_B[b]) {
        if (str_A[a++]!=str_B[b++]) {
            return false;
        }
    }
    while (str_A[a++]) {}
    while (str_B[b++]) {}
    if (a==b) {
        return true;
    }
    return false;
}
int lenString(Strings str) {
    int len=-1;
    while(str[++len]) {
        // do nothing
    }
    return len;
}
void cpyString(char *str, const int size, Strings content) {
    int len=0;
    while (content[len] && len<size) {
        str[len] = content[len];
        len++;
    }
    str[size] = 0;
}

bool isLowercase(const char ch) {
    if (ch>='a' && ch<='z') {
        return true;
    }
    return false;
}
bool isUppercase(const char ch) {
    if(ch>='A' && ch<='Z') {
        return true;
    }
    return false;
}
bool isNumber(const char ch) {
    if(ch>='0' && ch<='9') {
        return true;
    }
    return false;    
}
bool isSpecial(const char ch) {
    // 32–47 || 58–64 || 91–96 || 123–126
    if ((ch>=char(32) && ch<=char(47))
        or (ch>=char(58) && ch<=char(64))
        or (ch>=char(91) && ch<=char(96))
        or (ch>=char(123) && ch<=char(126))) {
        return true;
    }
    return false;
}

// username can include lowercase letter (a-z), numberic letter (0-9) and underscore (_)
// username has at least 5 character long and 20 character maximum
bool checkIfValidUsrnm(Strings username, const int lenUsr) {
    if (isLowercase(username[0])==false) {
        if (username[0]!='_') {
            std::cout << "username has begin with a lowercase letter or underscore!" << std::endl;
            return false;
        }
    }
    if (lenUsr < 5) {
        std::cout << "username has at least 5 character long!" << std::endl;
        return false;
    }
    int index = -1;
    while (username[++index]) {
        if (isUppercase(username[index])==true) {
            std::cout << "username can not include any uppercase letter!" << std::endl;
            return false;
        }
        else if (isSpecial(username[index])==true) {
            if (username[index]=='_') {
                continue;
            }
            std::cout << "username can not include any special letter except underscore!" << std::endl;
            return false;
        }
    }
    return true;
}
// password has to include both uppercase (A-Z) and lowercase letter (a-z)
// password has to include a digit (0-9)
// password has at least 1 special character
// password has not to contain username
bool checkIfStrongPasswd(Strings password, const int lenPwd) {
    if (lenPwd < 8) {
        printFormat("\t<!> Password too short!\n", YELLOW, ITALIC);
        printFormats("\t\tAn 8-character password is recommended\n", YELLOW, ITALIC, LIGHT);
        return false;
    }
    bool checkLow = false;
    bool checkUpp = false;
    bool checkNum = false;
    bool checkSpe = false;
    int index = -1;
    while (password[++index]) {
        if (checkLow==false) {
            checkLow = isLowercase(password[index]);
        }
        if (checkUpp==false) {
            checkUpp = isUppercase(password[index]);
        }
        if (checkNum==false) {
            checkNum = isNumber(password[index]);
        }
        if (checkSpe==false) {
            checkSpe = isSpecial(password[index]);
        }
    }
    if (checkLow==false or checkUpp==false or checkNum==false or checkSpe==false) {
        printFormat("\t<!> Password weakless\n", YELLOW, ITALIC);
        printFormat("\tHINT: Password doesn't contain:\n", MAGENTA, ITALIC);
        if(checkLow==false) {
            printFormat("\t\t(*) a lowercase letter\n", MAGENTA, ITALIC);
        }
        if(checkUpp==false) {
            printFormat("\t\t(*) an uppercase letter\n", MAGENTA, ITALIC);
        }
        if(checkNum==false) {
            printFormat("\t\t(*) a digit from 0 to 9\n", MAGENTA, ITALIC);
        }
        if(checkSpe==false) {
            printFormat("\t\t(*) a special letter\n", MAGENTA, ITALIC);
        }
        return false;
    }
    else {
        return true;
    }
}
// check if password contains username
bool checkIfContainUsrnm(Strings password, Strings username, const int lenUsr) {
    // check if password include username
    int index = -1;
    while (password[++index]) {
        int indexPwd = index;
        int indexUsr = 0;
        // the loop is stop if found any different
        while (password[indexPwd++]==username[indexUsr] && username[indexUsr]) {
            indexUsr++;
        }
        if (indexUsr==lenUsr)
        {
            // found that password include username
            printFormat("\t<!> Password should not contain username\n\n", YELLOW, ITALIC);
            return true;
        }
    }
    return false;
}

void signUp(char* username, char* password, char* fullname) {

    printFormats("\t >> Enter your FULLNAME: ", CYAN, BOLD, ITALIC);
    cinString(fullname, FULL_NAME_LEN);
    if (fullname[0]==ESC || fullname[0]==EXIT) {
        return;
    }

    int lenUsr = 0;
    printFormats("\t >> Enter your USERNAME: ", CYAN, BOLD, ITALIC);
    while (true) {
        std::cout << SAVE_CURSOR;
        std::cout << username;
        lenUsr = cinString(username, USER_NAME_LEN);
        if (username[0]==ESC || username[0]==EXIT) {
            return;
        }
        std::cout << CLEAR_AFTER;
        if (checkIfValidUsrnm(username, lenUsr)==false) {
            rstString(username, USER_NAME_LEN);
            std::cout << RESTORE_CURSOR << CLEAR_AFTER_INLINE;
        }
        else {
            break;
        }
    }

    int lenPwd = 0;
    rstString(repasswd, PASS_WORD_LEN);
    int lenRpw;
    printFormat("\t<NOTE> Password must have: lowercase, UPPERCASE, number 0..9, special-character\n", WHITE, ITALIC);
    printFormats("\t >> Enter your PASSWORD: ", CYAN, BOLD, ITALIC);
    while (true) {
        std::cout << SAVE_CURSOR;
        lenPwd = cinString(password, PASS_WORD_LEN, HIDE);
        if (password[0]==ESC || password[0]==EXIT) {
            return;
        }
        std::cout << CLEAR_AFTER;
        if (checkIfStrongPasswd(password, lenPwd)==false) {
            std::cout << RESTORE_CURSOR << CLEAR_AFTER_INLINE;
            rstString(password, PASS_WORD_LEN);
        }
        else if (checkIfContainUsrnm(password, username, lenUsr)==true) {
            std::cout << RESTORE_CURSOR << CLEAR_AFTER_INLINE;
            rstString(password, PASS_WORD_LEN);
        }
        else {
            printFormats("\t >> Retype the PASSWORD: ", CYAN, BOLD, ITALIC);
            lenRpw = cinString(repasswd, PASS_WORD_LEN, HIDE);
            if (repasswd[0]==ESC || repasswd[0]==EXIT) {
                return;
            }
            std::cout << CLEAR_AFTER;
            if (cmpString(password, repasswd)==false) {
                std::cout << BACK_LINE << CLEAR_ALL_INLINE;
                printFormat("\t<!> Password not match", YELLOW, ITALIC);
                std::cout << RESTORE_CURSOR << CLEAR_AFTER_INLINE;
                rstString(password, PASS_WORD_LEN);
                rstString(repasswd, PASS_WORD_LEN);
            }
            else {
                break;
            }
        }
    }
}

void login(char* username, char* password) {
    printFormats("\t >> Enter your USERNAME: ", CYAN, BOLD, ITALIC);
    std::cout << username;
    cinString(username, USER_NAME_LEN);
    if (username[0]==ESC || username[0]==EXIT) {
        return;
    }

    printFormats("\t >> Enter your PASSWORD: ", CYAN, BOLD, ITALIC);
    cinString(password, PASS_WORD_LEN, HIDE);
    if (password[0]==ESC || username[0]==EXIT) {
        return;
    }
}

void changePassword(char* password, Strings my_hashpass, Strings my_username) {

    printFormats("\t >> Enter your password: ", CYAN, BOLD, ITALIC);
    std::cout << SAVE_CURSOR;
    while (true) {
        cinString(password, PASS_WORD_LEN, HIDE);
        if (password[0]==ESC || password[0]==EXIT) {
            return;
        }
        cpyString(hashpass, HASH_PASS_LEN, sha256(password).c_str());
        // if (cmpString(my_hashpass, hashpass)==true) {
        //     break;
        // }
        // else {
        //     printFormat("\t<!> Password not match", YELLOW, ITALIC);
        //     rstString(password, PASS_WORD_LEN);
        // }
        // std::cout << RESTORE_CURSOR << CLEAR_AFTER_INLINE;
    }

    int lenUsr = lenString(my_username);
    int lenPwd = 0;
    rstString(password, PASS_WORD_LEN);
    int lenRpw;
    printFormat("\t<NOTE> Password must have: lowercase, UPPERCASE, number 0..9, special-character\n", WHITE, ITALIC);
    while (true) {
        std::cout << SAVE_CURSOR;
        printFormats("\t >> Enter your new PASSWORD: ", CYAN, BOLD, ITALIC);
        lenPwd = cinString(password, PASS_WORD_LEN, HIDE);
        if (password[0]==ESC || password[0]==EXIT) {
            return;
        }
        std::cout << CLEAR_AFTER;
        if (checkIfStrongPasswd(password, lenPwd)==false) {
            std::cout << RESTORE_CURSOR << CLEAR_ALL_INLINE;
            rstString(password, PASS_WORD_LEN);
        }
        else if (checkIfContainUsrnm(password, my_username, lenUsr)==true) {
            std::cout << RESTORE_CURSOR << CLEAR_ALL_INLINE;
            rstString(password, PASS_WORD_LEN);
        }
        else {
            printFormats("\t >> Retype the new PASSWORD: ", CYAN, BOLD, ITALIC);
            lenRpw = cinString(repasswd, PASS_WORD_LEN, HIDE);
            if (repasswd[0]==ESC || repasswd[0]==EXIT) {
                return;
            }
            std::cout << CLEAR_AFTER;
            if (cmpString(password, repasswd)==false) {
                std::cout << BACK_LINE << CLEAR_ALL_INLINE;
                printFormat("\t<!> New password not match", YELLOW, ITALIC);
                std::cout << BACK_LINE << CLEAR_ALL_INLINE;
                rstString(password, PASS_WORD_LEN);
                rstString(repasswd, PASS_WORD_LEN);
            }
            else {
                break;
            }
        }
    }
}
void changeColor(int &color) {
    printListColor();
    printFormats("\t >> Enter your COLOR MESSAGE: ", CYAN, BOLD, ITALIC);
    while (true) {
        color = cinChar(COLOR);
        if (color==ESC) {
            break;
        }
        else if (color==EXIT) {
            break;
        }
        else if (color>=30 && color<=39) {
            break;
        }
        else {
            continue;
        }
    }
}
void changeFullname(char* fullname) {
    printFormats("\t >> Enter your FULLNAME: ", CYAN, BOLD, ITALIC);
    cinString(fullname, FULL_NAME_LEN);
    if (fullname[0]==ESC || fullname[0]==EXIT) {
        return;
    }
}

void confirmChange(int &confirm) {
    int option = 0;
    printFormats("<?> Are you confirmed with these changes? [Y/N] ", RED, BOLD, ITALIC);
    while (option = cinChar(YESNO)) {
        if (option=='Y' || option=='y') {
            confirm=1;
            break;
        }
        else if (option==ESC || option=='N' || option=='n') {
            confirm=0;
            break;
        }
        else if (option==EXIT) {
            confirm=EXIT;
            break;
        }
    }
}

int  logout() {
    int confirm;
    int option = 0;
    printFormats("<?> Do you want to LogOut session [Y/N] ", RED, BOLD, ITALIC);
    while (option = cinChar(YESNO)) {
        if (option=='Y' || option=='y') {
            confirm=1;
            break;
        }
        else if (option==ESC || option=='N' || option=='n') {
            confirm=0;
            break;
        }
        else if (option==EXIT) {
            confirm=EXIT;
            break;
        }
    }
    return confirm;
}

int getArgumentInt(Strings command) {
    int value=0;
    int i=0;
    while (command[i++] != 32) {
        if (isNumber(command[i])==false) {
            return INVALID;
        }
        else {
            value = value*10 + (command[i]-48);
        }
        i++;
    }
    return value;
}
int getArgumentColor(Strings command) {
    if (findString(command, "black") > 0) {
        return BLACK;
    }
    else if (findString(command, "red") > 0) {
        return RED;
    }
    else if (findString(command, "green") > 0) {
        return GREEN;
    }
    else if (findString(command, "yellow") > 0) {
        return YELLOW;
    }
    else if (findString(command, "blue") > 0) {
        return BLUE;
    }
    else if (findString(command, "magenta") > 0) {
        return MAGENTA;
    }
    else if (findString(command, "cyan") > 0) {
        return CYAN;
    }
    else if (findString(command, "white") > 0) {
        return WHITE;
    }
    else {
        int value = getArgumentInt(command);
        if (value==INVALID) {
            return DEFAULT;
        }
        else {
            return (value%10 + 30);
        }
    }
}

void getConsoleSize(int& height, int& length) {

    rstString(buffer, 10);
    FILE* pipe = popen("stty size", "r");
    fgets(buffer, 10, pipe);
    pclose(pipe);

    int index = -1;
    height = 0;
    while (buffer[++index]!=32) {
        height = (height*10) + (buffer[index]-48);
    }
    length = 0;
    while (buffer[++index]!=10) {
        length = (length*10) + (buffer[index]-48);
    }
}
int getConsoleHeight() {
    rstString(buffer, 10);
    FILE* pipe = popen("stty size", "r");
    fgets(buffer, 10, pipe);
    pclose(pipe);

    int index = -1;
    int height = 0;
    while (buffer[++index]!=32) {
        height = (height*10) + (buffer[index]-48);
    }
    return height;
}
int getConsoleLength() {
    rstString(buffer, 10);
    FILE* pipe = popen("stty size", "r");
    fgets(buffer, 10, pipe);
    pclose(pipe);

    int index = -1;
    while (buffer[++index]!=32) {
        // do nothing
    }
    int length = 0;
    while (buffer[++index]!=10) {
        length = (length*10) + (buffer[index]-48);
    }
    return length;
}
void goodByeApp(int length) {
    int color= 0;
    while (color<10) {
        std::cout << CLEAR_ALL;
        std::cout << std::endl;
        std::cout << std::endl;
        printFormats(" ████╗████╗████╗ ██╗  ██╗████╗ ██╗ ██╗  ████╗  ████╗  ████╗ ██╗██╗ ██╗\n", (1+color)%7+31, REGULAR, REGULAR, CENTER, length);
        printFormats(" ██╔═╝██╔═╝██╔═╝ ╚██╗██╔██╔═██╗██║ ██║ ██╔═██╗██╔══╝ ██╔═██╗██║███╗██║\n", (2+color)%7+31, REGULAR, REGULAR, CENTER, length);
        printFormats(" ████╗████╗████╗  ╚███╔╝██║ ██║██║ ██║ ██████║██║███╗██████║██║██████║\n", (3+color)%7+31, REGULAR, REGULAR, CENTER, length);
        printFormats(" ╚═██║██╔═╝██╔═╝  ╚██╔╝ ██║ ██║██║ ██║ ██╔═██║██║ ██║██╔═██║██║██╚███║\n", (4+color)%7+31, REGULAR, REGULAR, CENTER, length);
        printFormats(" ████║████╗████╗   ██║  ╚████╔╝╚████╔╝ ██║ ██║╚████╔╝██║ ██║██║██║╚██║\n", (5+color)%7+31, REGULAR, REGULAR, CENTER, length);
        printFormats(" ╚═══╝╚═══╝╚═══╝   ╚═╝   ╚═══╝  ╚═══╝  ╚═╝ ╚═╝ ╚═══╝ ╚═╝ ╚═╝╚═╝╚═╝ ╚═╝\n", (6+color)%7+31, REGULAR, REGULAR, CENTER, length);
        color++;
        usleep(75000);
    }
    std::cout << DEFAULT_FORMAT;
    std::cout << CLEAR_ALL;
}
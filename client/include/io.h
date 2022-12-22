#ifndef __IO_H_
#define __IO_H_

#include <iostream>
#include <iomanip>
#include <string.h>
#include <unistd.h>

#include "sha256.h"

typedef const char* Strings;
typedef int RoomID;

typedef int Color;
#define BLACK   30
#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define MAGENTA 35
#define CYAN    36
#define WHITE   37
#define UNKNOWN 38
#define DEFAULT 39

typedef int Format;
#define REGULAR   0
#define BOLD      1
#define LIGHT     2
#define ITALIC    3
#define UNDERLINE 4
#define STRIKETHR 9

typedef char Align;
#define RIGHT  'R'
#define LEFT   'L'
#define CENTER 'E'

typedef int Option;
#define INVALID -3
#define EXIT    -2
#define ESC     -1
#define NONE     0
#define DIGIT    1
#define YESNO    2
#define COLOR    3
#define ANYCH    4

typedef int Page;
#define EXIT_APP  0
#define START_APP 1
#define HOME_PAGE 2
#define CHAT_PAGE 3

#define HIDE true
#define UNHIDE false

#define USER_NAME_LEN   15
#define PASS_WORD_LEN   30
#define HASH_PASS_LEN   64
#define FULL_NAME_LEN   30
#define ROOM_NAME_LEN   30
#define NUMBER_LEN      8
#define MESSAGE_LEN     2048
#define REQUEST_LEN     2048
#define RESPONSE_LEN    2048

// move backward 1 column then clear all after cursor, cursor standstill
#define BACK_SPACE          "\033[D\033[K"
// move backward 2 column then clear all after cursor, cursor standstill
#define BACK_SIGNAL         "\033[2D\033[K"
// move up 1 line, cursor in the leftmost
#define BACK_LINE           "\033[F"

// clear all after cursor and also lines below, cursor standstill
#define CLEAR_AFTER         "\033[J"
// clear all console then puts cursor at top and leftmost
#define CLEAR_ALL           "\033[2J\033[H"
// clear all after cursor, cursor standstill
#define CLEAR_AFTER_INLINE  "\033[K"
// clear all in line, cursor standstill
#define CLEAR_ALL_INLINE    "\033[2K"

#define TAB             "\033[I"
#define HOME_LINE       "\033[L"
#define HOME            "\033[H"

#define SAVE_CURSOR     "\033[s"
#define RESTORE_CURSOR  "\033[u"

#define DEFAULT_FORMAT  "\033[0m"

#define COUNTDOWN_SEC 2
#define NOT_FOUND -1

#define FIRST_LINE  0
#define LAST_LINE   1
#define BODY_LINE   2
#define NOTICE_LINE 3

void printFrame(int, Strings, int);
void printMenu(int, int, int);

Strings colorName(int);
void printListColor();
void printFormats(Strings, Color, Format, Format, Align, int);
void printFormat(Strings, Color, Format, Align, int);
void printCountdown(Strings, int);

int cinChar(int);

int getValue(Strings);
int cinNumber();
int numDigits(int);

int cinString(char*, const int, bool);

int findString(Strings, Strings);
void rstString(char*, const int);
bool cmpString(Strings, Strings);
int  lenString(Strings);
void cpyString(char*, const int, Strings);

bool isLowercase(const char);
bool isUppercase(const char);
bool isNumber(const char);
bool isSpecial(const char);

bool checkIfValidUsrnm(Strings, const int);
bool checkIfStrongPasswd(Strings, const int);
bool checkIfContainUsrnm(Strings, Strings, const int);

void signUp(char*, char*, char*);
void login(char*, char*);

void changePassword(char*, Strings, Strings);
void changeColor(int&);
void changeFullname(char*);

void confirmChange(int&);
int  logout();

int getArgumentInt(Strings);
int getArgumentColor(Strings);
void getConsoleSize(int&, int&);
int getConsoleHeight();
int getConsoleLength();
void goodByeApp(int);

#endif
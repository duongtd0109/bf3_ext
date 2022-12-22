# bf3_ext
## Request code
    SIGN_UP         : register a new account
    LOGIN           : login into the app
    FIND_USER       : find all of users
    FIND_ROOM       : find all of public room
    CHANGE_PASSWD   : change your password
    CHANGE_COLORS   : change your own chatting color
    CHANGE_FULLNM   : change your fullname (basic information)
    GET_INFO        : get some information abour this app
    REMSGS_USER     : retrieve messages about you and other user
    REMSGS_ROOM     : retrieve messages inside the room chat
    SEND_MSG        : send a new message from you
    LOGOUT          : logout of the app

## About \033 ANSI escape sequence

1. FORMATTING

    - \033[<color>m   -> set text/background color
    - \033[39m        -> reset text color
    - \033[49m        -> reset background color

    - \033[<format>m  -> turn on
    - \033[2<format>m -> turn off

    - \033[0m         -> reset all formatting

2. CURSOR

    - \033[<N>A   -> move the cursor up N lines
    - \033[<N>B   -> move the cursor down N lines
    - \033[<N>C   -> move the cursor forward N columns
    - \033[<N>D   -> move the cursor backward N columns
    
    - \033[<N>E   -> end N line, (cursor in beginning of the line)
    - \033[<N>F   -> go to forward N lines (cursor in beginning of the line)
    - \033[x;yH   -> puts the cursor at line x and column y
    - \033[<N>I   -> move N tab
    
    - \033[J      -> clear from the cursor to end of screen, cursor stand still
    - \033[2J     -> clear the screen, cursor stand still
    - \033[K      -> clear from the cursor to end of line, cursor stand still
    - \033[2K     -> clear line, cursor stand still

    - \033[s      -> save cursor position
    - \033[u      -> restore cursor position

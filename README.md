# bf3_ext
Document about \033 ANSI escape sequence

1. FORMATTING

\033[<color>m
    -> set text/background color
\033[39m
    -> reset text color
\033[49m
    -> reset background color

\033[<format>m
    -> turn on bold/italic/underline
\033[2<format>m
    -> turn off

\033[0m
    -> reset all formatting

2. CURSOR

\033[<N>A
    -> move the cursor up N lines
\033[<N>B
    -> move the cursor down N lines
\033[<N>C
    -> move the cursor forward N columns
\033[<N>D
    -> move the cursor backward N columns
\033[<N>E
    -> end N line, (cursor in beginning of the line)
\033[<N>F
    -> go to forward N lines (cursor in beginning of the line)
\033[<L>;<C>H
    -> puts the cursor at line L and column C
\033[<N>I
    -> move N tab
\033[J
    -> clear from the cursor to end of screen, cursor stand still
\033[2J
    -> clear the screen, cursor stand still
\033[K
    -> clear from the cursor to end of line, cursor stand still
\033[2K
    -> clear line, cursor stand still

\033[s
    -> save cursor position
\033[u
    -> restore cursor position

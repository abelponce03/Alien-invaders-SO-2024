#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

void intro(int win)
{
    nodelay(stdscr, 0);

    clear();
    move((LINES / 2) - 1, (COLS / 2) - 5);

    addstr("");

    move((LINES / 2), (COLS / 2) - 11);
    addstr("PRESS ANY KEY TO START");
    move(0, COLS - 1);
    refresh();
    getch();
}
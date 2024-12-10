#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

//Esta funcion maneja lo que sucede si ganas o pierdes

void gameover(int win)
{
    nodelay(stdscr, 0);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    if(win == 0)
    {
        clear();
        move((LINES/2) - 5, (COLS/2) -75);
        attron(COLOR_PAIR(6));
        addstr(".______    _______ .______       _______   __       _______..___________. _______    .______    __    __  .__   __. .___________.  ______   \n");
        move((LINES/2) - 4, (COLS/2) - 75);
        attron(COLOR_PAIR(4));
        addstr("|   _  \\  |   ____||   _  \\     |       \\ |  |     /       ||           ||   ____|   |   _  \\  |  |  |  | |  \\ |  | |           | /  __  \\  \n");
        move((LINES/2) - 3, (COLS/2) - 75);
        attron(COLOR_PAIR(5));
        addstr("|  |_)  | |  |__   |  |_)  |    |  .--.  ||  |    |   (----``---|  |----`|  |__      |  |_)  | |  |  |  | |   \\|  | `---|  |----`|  |  |  | \n");
        move((LINES/2) - 2, (COLS/2) - 75);
        attron(COLOR_PAIR(1));
        addstr("|   ___/  |   __|  |      /     |  |  |  ||  |     \\   \\        |  |     |   __|     |   ___/  |  |  |  | |  . `  |     |  |     |  |  |  | \n");
        move((LINES/2) - 1, (COLS/2) - 75);
        attron(COLOR_PAIR(2));
        addstr("|  |      |  |____ |  |\\  \\----.|  '--'  ||  | .----)   |       |  |     |  |____    |  |      |  `--'  | |  |\\   |     |  |     |  `--'  | \n");
        move((LINES/2), (COLS/2) - 75);
        attron(COLOR_PAIR(3));
        addstr("| _|      |_______|| _| `._____||_______/ |__| |_______/        |__|     |_______|   | _|       \\______/  |__| \\__|     |__|      \\______/  \n");
        attron(COLOR_PAIR(7));
        move((LINES/2 + 5), (COLS/2) - 19);
        addstr("PRESS ANY KEY TO EXIT");
        move(0, COLS - 1);
        refresh();
        getch();
    }
    else if(win == 1)
    {
        clear();
        move((LINES/2) - 5, (COLS/2) - 65);
        attron(COLOR_PAIR(6));
        addstr(" _______ .______       _______      _______.   .___________.     ___      .__   __.   ______       __    __   _______ \n");
        move((LINES/2) - 4, (COLS/2) - 65);
        attron(COLOR_PAIR(4));
        addstr("|   ____||   _  \\     |   ____|    /       |   |           |    /   \\     |  \\ |  |  /  __  \\     |  |  |  | |   ____|\n");
        move((LINES/2) - 3, (COLS/2) - 65);
        attron(COLOR_PAIR(5));
        addstr("|  |__   |  |_)  |    |  |__      |   (----`   `---|  |----`   /  ^  \\    |   \\|  | |  |  |  |    |  |  |  | |  |__   \n");
        move((LINES/2) - 2, (COLS/2) - 65);
        attron(COLOR_PAIR(1));
        addstr("|   __|  |      /     |   __|      \\   \\           |  |       /  /_\\  \\   |  . `  | |  |  |  |    |  |  |  | |   __|  \n");
        move((LINES/2) - 1, (COLS/2) - 65);
        attron(COLOR_PAIR(2));
        addstr("|  |____ |  |\\  \\----.|  |____ .----)   |          |  |      /  _____  \\  |  |\\   | |  `--'  '--. |  `--'  | |  |____ \n");
        move((LINES/2), (COLS/2) - 65);
        attron(COLOR_PAIR(3));
        addstr("|_______|| _| `._____||_______||_______/           |__|     /__/     \\__\\ |__| \\__|  \\_____\\_____\\ \\______/  |_______|\n");
        attron(COLOR_PAIR(7));
        move((LINES/2 + 5), (COLS/2) - 19);
        addstr("PRESS ANY KEY TO EXIT");
        move(0, COLS - 1);
        refresh();
        getch();
    }
}

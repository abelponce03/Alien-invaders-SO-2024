#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

// Esta funcion maneja el menu de opciones disponibles para el jugador

void menu(struct options *settings)
{
    char option, buf[30];
    int new;

    clear();
    echo();
    nocbreak();
    nodelay(stdscr, 0);

    attron(COLOR_PAIR(1));
    move(0, 0);
    addstr("  _          ___                             _   _                                                              _ \n");
    addstr(" / |        / _ \\  __ __  ___   _ _   __ _  | | | |    __ _   __ _   _ __    ___     ___  _ __   ___   ___   __| |\n");
    addstr(" | |  _    | (_) | \\ V / / -_) | '_| / _` | | | | |   / _` | / _` | | '  \\  / -_)   (_-< | '_ \\ / -_) / -_) / _` |\n");
    addstr(" |_| (_)    \\___/   \\_/  \\___| |_|   \\__,_| |_| |_|   \\__, | \\__,_| |_|_|_| \\___|   /__/ | .__/ \\___| \\___| \\__,_|\n");
    addstr("                                                      |___/                              |_|                      \n");

    attron(COLOR_PAIR(2));
    move(4, 0);
    addstr("  ___           _     _   _                                             _ \n");
    addstr(" |_  )         /_\\   | | (_)  ___   _ _      ___  _ __   ___   ___   __| |\n");
    addstr("  / /   _     / _ \\  | | | | / -_) | ' \\    (_-< | '_ \\ / -_) / -_) / _` |\n");
    addstr(" /___| (_)   /_/ \\_\\ |_| |_| \\___| |_||_|   /__/ | .__/ \\___| \\___| \\__,_|\n");
    addstr("                                                 |_|                      \n");

    attron(COLOR_PAIR(3));
    move(9, 0);
    addstr("  ____        ___   _                  _                                  _ \n");
    addstr(" |__ /       / __| | |_    ___   ___  | |_     ___  _ __   ___   ___   __| |\n");
    addstr("  |_ \\  _    \\__ \\ | ' \\  / _ \\ / _ \\ |  _|   (_-< | '_ \\ / -_) / -_) / _` |\n");
    addstr(" |___/ (_)   |___/ |_||_| \\___/ \\___/  \\__|   /__/ | .__/ \\___| \\___| \\__,_|\n");
    addstr("                                                   |_|                      \n");

    attron(COLOR_PAIR(4));
    move(14, 0);
    addstr("  _ _          ___                 _                                   _ \n");
    addstr(" | | |        | _ )  ___   _ __   | |__     ___  _ __   ___   ___   __| |\n");
    addstr(" |_  _|  _    | _ \\ / _ \\ | '  \\  | '_ \\   (_-< | '_ \\ / -_) / -_) / _` |\n");
    addstr("   |_|  (_)   |___/ \\___/ |_|_|_| |_.__/   /__/ | .__/ \\___| \\___| \\__,_|\n");
    addstr("                                                |_|                      \n");

    attron(COLOR_PAIR(5));
    move(19, 0);
    addstr("  ___         ___                 _           _                             _                   ___   ___ \n");
    addstr(" | __|       | _ )  ___   _ __   | |__     __| |  _ _   ___   _ __   _ __  (_)  _ _    __ _    | __| | _ \\ \n");
    addstr(" |__ \\  _    | _ \\ / _ \\ | '  \\  | '_ \\   / _` | | '_| / _ \\ | '_ \\ | '_ \\ | | | ' \\  / _` |   | _|  |   / \n");
    addstr(" |___/ (_)   |___/ \\___/ |_|_|_| |_.__/   \\__,_| |_|   \\___/ | .__/ | .__/ |_| |_||_| \\__, |   |_|   |_|_\\ \n");
    addstr("                                                             |_|    |_|               |___/               \n");

    attron(COLOR_PAIR(6));
    move(24, 0);
    addstr("  __         ___         _                           _             _     _                                        \n");
    addstr(" / /        | _ \\  ___  | |_   _  _   _ _   _ _     | |_   ___    | |_  | |_    ___     __ _   __ _   _ __    ___ \n");
    addstr("/ _ \\  _    |   / / -_) |  _| | || | | '_| | ' \\    |  _| / _ \\   |  _| | ' \\  / -_)   / _` | / _` | | '  \\  / -_)\n");
    addstr("\\___/ (_)   |_|_\\ \\___|  \\__|  \\_,_| |_|   |_||_|    \\__| \\___/    \\__| |_||_| \\___|   \\__, | \\__,_| |_|_|_| \\___|\n");
    addstr("                                                                                       |___/                     \n");

    attron(COLOR_PAIR(7));
    move(29, 0);
    addstr(" ____         ___         _   _       _     _                            \n");
    addstr("|__  |       | __| __ __ (_) | |_    | |_  | |_    ___     __ _   __ _   _ __    __\n");
    addstr("  / /   _    | _|  \\ \\ / | | |  _|   |  _| | ' \\  / -_)   / _` | / _` | | '  \\  / -_)\n");
    addstr(" /_/   (_)   |___| /_\\_\\ |_|  \\__|    \\__| |_||_| \\___|   \\__, | \\__,_| |_|_|_| \\___|\n");
    addstr("                                                           |___/              \n");

    move(34, 120);
    addstr("Enter your option: ");

    move(4, 135);
    addstr("                    .   ,- \n");
    move(5, 135);
    addstr("                   .'.\n");
    move(6, 135);
    addstr("                   |o|\n");
    move(7, 135);
    addstr("                  .'o'.\n");
    move(8, 135);
    addstr("                  |.-.|\n");
    move(9, 135);
    addstr("                  '   '\n");
    move(10, 135);
    addstr("                   ( )\n");
    move(11, 135);
    addstr("                    )\n");
    move(12, 135);
    addstr("                   ( )\n");
    move(13, 135);
    addstr("               _____\n");
    move(14, 135);
    addstr("          .-'\"\"p 8o\"\" -.\n");
    move(15, 135);
    addstr("       .-'8888P'Y.Y[ '     -.\n");
    move(16, 135);
    addstr("     ,']88888b.J8oo_         '.\n");
    move(17, 135);
    addstr("   ,' ,88888888888[\"          Y.\n");
    move(18, 135);
    addstr("  /   8888888888P             Y8\\\n");
    move(19, 135);
    addstr(" /    Y8888888P'              ]88\\\n");
    move(20, 135);
    addstr(":     Y88'   P                 888:\n");
    move(21, 135);
    addstr(":       Y8.oP '- >             Y88:\n");
    move(22, 135);
    addstr("|          Yb  __                '|\n");
    move(23, 135);
    addstr(":            'd8888bo.            :\n");
    move(24, 135);
    addstr(":             d88888888ooo.      ;\n");
    move(25, 135);
    addstr(" \\            Y88888888888P      /\n");
    move(26, 135);
    addstr("  \\            Y88888888P       /\n");
    move(27, 135);
    addstr("   .            d88888P'      ,'\n");
    move(28, 135);
    addstr("     .          888PP'      ,'\n");
    move(29, 135);
    addstr("       -.      d8P'      ,-'   \n");
    move(30, 135);
    addstr("          -.,,_'__,,.-'\n");

    refresh();

    while (1)
    {
        move(35, 120);
        addstr("                                                                 ");
        move(36, 120);
        addstr("                                                                 ");
        move(34, 139);
        option = getch();

        if (option == '1')
        {
            sprintf(buf, "Curret value: %d\n", settings->overall);

            move(35, 120);
            addstr(buf);
            move(36, 120);
            addstr("Enter new value: ");
            move(36, 137);
            refresh();
            getch();
            getstr(buf);

            new = atoi(buf);
            // Verificar nuevo valor
            if (new < 0)
            {
                move(37, 120);
                addstr("ERROR: Invalid value");
            }
            else
                settings->overall = new;

            move(34, 139);
            addstr("                                                ");
        }

        else if (option == '2')
        {
            sprintf(buf, "Curret value: %d\n", settings->enemigo);

            move(35, 120);
            addstr(buf);
            move(36, 120);
            addstr("Enter new value: ");
            move(36, 137);
            refresh();
            getch();
            getstr(buf);

            new = atoi(buf);
            // Verificar nuevo valor
            if (new <= 0)
            {
                move(37, 120);
                addstr("ERROR: Invalid value");
            }
            else
                settings->enemigo = new;

            move(34, 139);
            addstr("                                                 ");
        }

        else if (option == '3')
        {
            sprintf(buf, "Curret value: %d\n", settings->disparos);

            move(35, 120);
            addstr(buf);
            move(36, 120);
            addstr("Enter new value: ");
            move(36, 137);
            refresh();
            getch();
            getstr(buf);

            new = atoi(buf);
            // Verificar nuevo valor
            if (new <= 0)
            {
                move(37, 120);
                addstr("ERROR: Invalid value");
            }
            else
                settings->disparos = new;

            move(34, 139);
            addstr("                                                   ");
        }

        else if (option == '4')
        {
            sprintf(buf, "Curret value: %d\n", settings->bombas);

            move(35, 120);
            addstr(buf);
            move(36, 120);
            addstr("Enter new value: ");
            move(36, 137);
            refresh();
            getch();
            getstr(buf);

            new = atoi(buf);
            // Verificar nuevo valor
            if (new <= 0)
            {
                move(37, 120);
                addstr("ERROR: Invalid value");
            }
            else
                settings->bombas = new;

            move(34, 139);
            addstr("                                                    ");
        }
        else if (option == '5')
        {
            sprintf(buf, "Curret value: %d\n", settings->chanceBomba);

            move(35, 120);
            addstr(buf);
            move(36, 120);
            addstr("Enter new value: ");
            move(36, 137);
            refresh();
            getch();
            getstr(buf);

            new = atoi(buf);
            // Verificar nuevo valor
            if (new > 100 || new < 0)
            {
                move(37, 120);
                addstr("ERROR: Invalid value");
            }
            else
                settings->chanceBomba = new;

            move(34, 139);
            addstr("                                                                 ");
        }

        else if (option == '6')
            break;

        else if (option == '7')
        {
            endwin();
            exit(0);
            break;
        }

        else
        {
            move(35, 120);
            addstr("ERROR: Invalid selection");
            move(34, 139);
            addstr(" ");
            refresh();
        }
    }

    clear();
    noecho();
    cbreak();
    nodelay(stdscr, 1);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);

    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    // Titulo del juego, puntuacion, encabezado y opciones
    attron(COLOR_PAIR(5));
    move(0, (COLS / 2) - 50);
    addstr("  __  __          _                              ___                            _              \n");
    attron(COLOR_PAIR(4));
    move(1, (COLS / 2) - 50);
    addstr(" |  \\/  |  __ _  | |_   __   ___   _ __    ___  |_ _|  _ _   __ __  __ _   ___ (_)  ___   _ _  \n");
    attron(COLOR_PAIR(3));
    move(2, (COLS / 2) - 50);
    addstr(" | |\\/| | / _` | |  _| / _| / _ \\ | '  \\  |___|  | |  | ' \\  \\ V / / _` | (_-< | | / _ \\ | ' \\ \n");
    attron(COLOR_PAIR(1));
    move(3, (COLS / 2) - 50);
    addstr(" |_|  |_| \\__,_|  \\__| \\__| \\___/ |_|_|_|       |___| |_||_|  \\_/  \\__,_| /__/ |_| \\___/ |_||_| \n");

    move(0, 19);
    attron(COLOR_PAIR(1));
    addstr("X X X");

    move(0, COLS - 19);
    attron(COLOR_PAIR(4));
    addch('M');
    attron(COLOR_PAIR(7));
    addstr(" = MENU");
    move(0, COLS - 10);
    attron(COLOR_PAIR(2));
    addch('Q');
    attron(COLOR_PAIR(7));
    addstr(" = SALIR");

    move(0, 3);
    addstr("SCORE: ");

    for (int i = 4; i < COLS - 3; i++)
    {
        move(5, i);
        addch('_');
        move(LINES - 1, i);
        addch('_');
    }
    for (int i = 6; i < LINES; i++)
    {
        move(i, 3);
        addch('|');
        move(i, COLS - 3);
        addch('|');
    }
}

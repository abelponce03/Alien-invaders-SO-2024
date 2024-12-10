#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include "Estructuras.c"
#include "Menu.c"
#include "Gameover.c"
#include "Enemigo.h"
#include <pthread.h>
#include <semaphore.h>
#include <dispatch/dispatch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define Max_BOMBAS 1000
#define ALIEN_COLOR_1 1
#define ALIEN_COLOR_2 2
#define ALIEN_COLOR_3 3
#define ALIEN_COLOR_4 4
#define ALIEN_COLOR_5 5
#define ALIEN_COLOR_6 6
#define NAVE_COLOR 7

struct nave nave;
struct disparo disparo[3];
enemigo *enemy = NULL;
enemigo *lastEnemy = NULL;
struct bomba bomba[Max_BOMBAS];
struct options options;
static int input, loops = 0, win = -1, Disparos_Actuales = 0, Enemigos_Actuales = 30, Bombas_Actuales = 0, mi_random = 0, score = 0, naveVida = 3;
static char tellscore[30];
int enemy_count = 0;

// Lista de colas que almacenaran a los enemigos

// Definicion de semaforos para cuidar los puntos criticos

static dispatch_semaphore_t sem_enemigo_disparo_bomba;

// Definicion de varios hilos
pthread_t mover_enemigo_thread, mover_disparo_thread, mover_bomba_thread;
pthread_t entrada_usuario_nave_thread, entrada_usuario_disparo_thread;

// Hilos para enemigos
pthread_t creacion_enemigo;

Mix_Chunk *sonido_disparo;

Mix_Chunk *sonido_colision;

Mix_Chunk *sonido_Fondo;

Mix_Chunk *sonido_Win;

Mix_Chunk *sonido_gameOver;

#define MEMORY_SIZE 1024 // Tamaño total de la memoria simulada

typedef struct Block
{
    int size;
    int start;
    struct Block *next;
} Block;

char memory[MEMORY_SIZE]; // Memoria simulada
Block *freeList = NULL;

void menu(struct options *settings);
void gameover(int win);
void *MOVER_DISPAROS();
void *MOVER_BOMBAS();
void DrawEnemy(enemigo *e);
void *MOVER_ENEMIGOS();
void *VERIFICAR_END_GAME();
void *VERIFICAR_ENTRADA_MOV_NAVE();
void *VERIFICAR_ENTRADA_DISPARAR();
void *VERIFICAR_ENTRADA_OTROS(struct options options, int *win);
void *INICIALIZAR();
void *Create_enemys();
void *Enemy_generator();
void free_enemys();
void clouseAudio();
void playSoud_Game();

int main()
{
    INICIALIZAR();

    while (win == -1)
    {
        dispatch_semaphore_wait(sem_enemigo_disparo_bomba, DISPATCH_TIME_FOREVER);

        sprintf(tellscore, "%d", score);
        move(0, 10);
        addstr(tellscore);

        // Mover nave
        move(nave.y, nave.x);
        addch(nave.ch);

        dispatch_semaphore_signal(sem_enemigo_disparo_bomba);

        // Estas condicionales son para que el movimiento de estos objetos sea mas lento

        if (loops % options.bombas == 0)
            pthread_create(&mover_bomba_thread, NULL, MOVER_BOMBAS, NULL);

        if (loops % options.disparos == 0)
            pthread_create(&mover_disparo_thread, NULL, MOVER_DISPAROS, NULL);

        if (loops % options.enemigo == 0)
            pthread_create(&mover_enemigo_thread, NULL, MOVER_ENEMIGOS, NULL);

        // Verificar si ganaste o perdiste el juego

        VERIFICAR_END_GAME();

        // Refrescar y tomar lo que escribe el usuario
        usleep(options.overall);

        input = getch();

        loops++;

        // Verificaciones de entrada de usuario
        pthread_create(&entrada_usuario_nave_thread, NULL, VERIFICAR_ENTRADA_MOV_NAVE, NULL);

        pthread_create(&entrada_usuario_disparo_thread, NULL, VERIFICAR_ENTRADA_DISPARAR, NULL);
        VERIFICAR_ENTRADA_OTROS(options, &win);

        // Espera de hilos
        pthread_join(mover_bomba_thread, NULL);
        pthread_join(mover_enemigo_thread, NULL);
        pthread_join(mover_disparo_thread, NULL);
    }

    gameover(win);
    free_enemys();
    clouseAudio();
    endwin();
    return 0;
}

//---------------------------------------------------------------------------------------------------------
// Metodo para la seleccion de tipo de enemigo
char Random_type()
{
    int random = rand() % 21;

    if (random == 0)
        return '!';
    else if (random == 1)
        return '@';
    else if (random == 2)
        return '#';
    else if (random == 3)
        return '$';
    else if (random == 4)
        return '%';
    else if (random == 5)
        return '&';
    else if (random == 6)
        return '?';
    else if (random == 7)
        return '{';
    else if (random == 8)
        return '}';
    else if (random == 9)
        return '[';
    else if (random == 10)
        return 'A';
    else if (random == 11)
        return 'B';
    else if (random == 12)
        return 'E';
    else if (random == 13)
        return 'L';
    else if (random == 14)
        return 'M';
    else if (random == 15)
        return 'U';
    else if (random == 16)
        return 'R';
    else if (random == 17)
        return 'I';
    else if (random == 18)
        return 'C';
    else if (random == 19)
        return '0';
    else
        return ']';
}

// Metodos q anadi para simular la linked list

void initialize_memory()
{
    freeList = (Block *)malloc(sizeof(Block));
    freeList->size = MEMORY_SIZE;
    freeList->start = 0;
    freeList->next = NULL;
}

void *first_fit_allocate(int size)
{
    Block *current = freeList;
    Block *previous = NULL;

    while (current != NULL)
    {
        if (current->size >= size)
        {
            // Asignar memoria
            void *allocated_memory = &memory[current->start]; //direccion de memoria

            // Actualizar la lista de bloques libres
            current->start += size;
            current->size -= size;

            if (current->size == 0)
            {
                // Eliminar el bloque si está completamente utilizado
                if (previous == NULL)
                {
                    freeList = current->next;
                }
                else
                {
                    previous->next = current->next;
                }
                free(current);
            }
            return allocated_memory;
        }
        previous = current;
        current = current->next;
    }
    return NULL; // No hay suficiente memoria
}

void Add_enemy2()
{
    enemigo *new_enemy = (enemigo *)first_fit_allocate(sizeof(enemigo));

    if (new_enemy == NULL)
    {
        return;// no hay espacio en memoria para anadir otro enemigo si eliminas enemigos entonces se vuelven a generar
    }

    new_enemy->direccion = rand() % 2;
    new_enemy->y = 6;
    new_enemy->x = (rand() % (COLS - 5)) + 5;
    new_enemy->vivo = 1;
    new_enemy->ch = Random_type();
    new_enemy->next = NULL;

    // Insertar al final de la lista de enemigos
    if (enemy == NULL)
    {
        enemy = new_enemy;
        lastEnemy = new_enemy;
    }
    else
    {
        lastEnemy->next = new_enemy;
        lastEnemy = new_enemy;
    }
}

void *Create_enemys(int number_enemys)
{
    int n = number_enemys;
    for (int i = 0; i < n; i++)
    {
        Add_enemy2();
    }
}

void *Enemy_generator()
{
    while (1)
    {
        dispatch_semaphore_wait(sem_enemigo_disparo_bomba, DISPATCH_TIME_FOREVER);
        Create_enemys(5);
        dispatch_semaphore_signal(sem_enemigo_disparo_bomba);
        sleep(3);
    }
}

void free_memory(void *ptr, int size)
{
    Block *newBlock = (Block *)malloc(sizeof(Block));
    newBlock->size = size;
    newBlock->start = (char *)ptr - memory;
    newBlock->next = freeList;
    freeList = newBlock;
}

void Delete_enemy(enemigo *delete)
{
    enemigo *current = enemy;
    enemigo *prev = current;

    if (delete == enemy)
    {
        enemy = delete->next;
        free_memory(delete, sizeof(enemigo)); // Liberar memoria usando free_memory()
    }
    else
    {
        while (current->next != delete)
        {
            current = current->next;
        }
        prev = current;
        current = current->next;

        if (current == lastEnemy)
        {
            lastEnemy = prev;
        }

        prev->next = current->next;
        free_memory(current, sizeof(enemigo)); // Liberar memoria usando free_memory()
    }
}

void free_enemys()
{
    enemigo *current = enemy;
    enemigo *next;

    while (current != NULL)
    {
        next = current->next;
        free_memory(current, sizeof(enemigo)); // Liberar memoria usando free_memory()
        current = next;
    }

    enemy = NULL;
    lastEnemy = NULL;
}

void DrawEnemy(enemigo *e)
{
    int x = e->x;
    int y = e->y;

    move(y, x);
    addch(e->ch);
}
// Implementacion para el audio

void initAudio()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Error inicializando SDL: %s\n", SDL_GetError());
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Error inicializando SDL_mixer: %s\n", Mix_GetError());
        exit(1);
    }

    sonido_disparo = Mix_LoadWAV("disparo.wav");

    if (!sonido_disparo)
    {
        fprintf(stderr, "Error cargando sonido: %s \n", Mix_GetError());
        exit(1);
    }

    sonido_colision = Mix_LoadWAV("colision.wav");

    if (!sonido_colision)
    {
        fprintf(stderr, "Error cargando sonido: %s \n", Mix_GetError());
        exit(1);
    }

    sonido_Fondo = Mix_LoadMUS("laserTag2.wav");

    if (!sonido_Fondo)
    {
        fprintf(stderr, "Errord cargando sonido: %s \n", Mix_GetError());
        exit(1);
    }

    sonido_Win = Mix_LoadWAV("arcade-game-winner.mp3");

    if (!sonido_Win)
    {
        fprintf(stderr, "Error cargando sonido: %s \n", Mix_GetError());
        exit(1);
    }

    sonido_gameOver = Mix_LoadWAV("game-over.wav");

    if (!sonido_gameOver)
    {
        fprintf(stderr, "Error cargando sonido: %s \n", Mix_GetError());
        exit(1);
    }
}

void clouseAudio()
{
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    Mix_FreeChunk(sonido_disparo);
    Mix_FreeChunk(sonido_colision);
    Mix_FreeMusic(sonido_Fondo);
    Mix_FreeChunk(sonido_gameOver);
    Mix_FreeChunk(sonido_Win);
    Mix_CloseAudio();
    SDL_Quit();
}

void playSoud_Game()
{
    if (Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(sonido_Fondo, -1);
    }
}
void playSoud_GameOver()
{
    Mix_PlayChannel(-1, sonido_gameOver, 0);
}
void playSoud_Win()
{
    Mix_PlayChannel(-1, sonido_Win, 0);
}
void playSoud_Shoot()
{
    Mix_PlayChannel(-1, sonido_disparo, 0);
}

void playSoud_Impact()
{
    Mix_PlayChannel(-1, sonido_colision, 0);
}

//---------------------------------------------------------------------------------------------------------------------
void *INICIALIZAR()
{
    // inicializacion de semaforos
    initialize_memory();
    sem_enemigo_disparo_bomba = dispatch_semaphore_create(1);

    initscr();
    initAudio();
    playSoud_Game();
    start_color();
    curs_set(0);
    clear();
    noecho();
    cbreak();
    nodelay(stdscr, 1);
    keypad(stdscr, 1);
    srand(time(NULL));

    // Colores

    init_pair(ALIEN_COLOR_1, COLOR_RED, COLOR_BLACK);
    init_pair(ALIEN_COLOR_2, COLOR_GREEN, COLOR_BLACK);
    init_pair(ALIEN_COLOR_3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(ALIEN_COLOR_4, COLOR_CYAN, COLOR_BLACK);
    init_pair(ALIEN_COLOR_5, COLOR_BLUE, COLOR_BLACK);
    init_pair(ALIEN_COLOR_6, COLOR_YELLOW, COLOR_BLACK);

    init_pair(NAVE_COLOR, COLOR_WHITE, COLOR_BLACK);

    // Opciones

    options.overall = 15000;
    options.enemigo = 12;
    options.disparos = 3;
    options.bombas = 10;
    options.chanceBomba = 5;

    // Nave

    nave.y = LINES - 2;
    nave.x = COLS / 2;

    nave.ch = '^';

    // Enemigos

    // Implementacion de enemigos nueva
    pthread_create(&creacion_enemigo, NULL, Enemy_generator, NULL);

    enemigo *current = enemy;
    while (current != NULL)
    {
        dispatch_semaphore_wait(sem_enemigo_disparo_bomba, DISPATCH_TIME_FOREVER);
        DrawEnemy(current);
        dispatch_semaphore_signal(sem_enemigo_disparo_bomba);
        current = current->next;
    }

    refresh();

    // Disparos

    for (int i = 0; i < 3; i++)
    {
        disparo[i].activo = 0;
        disparo[i].ch = '*';
    }

    // Bombas

    for (int i = 0; i < Max_BOMBAS; i++)
    {
        bomba[i].activo = 0;
        bomba[i].ch = 'o';
    }

    // Titulo del juego, puntuacion, encabezado y opciones
    attron(COLOR_PAIR(ALIEN_COLOR_5));
    move(0, (COLS / 2) - 50);
    addstr("  __  __          _                              ___                            _              \n");
    attron(COLOR_PAIR(ALIEN_COLOR_4));
    move(1, (COLS / 2) - 50);
    addstr(" |  \\/  |  __ _  | |_   __   ___   _ __    ___  |_ _|  _ _   __ __  __ _   ___ (_)  ___   _ _  \n");
    attron(COLOR_PAIR(ALIEN_COLOR_3));
    move(2, (COLS / 2) - 50);
    addstr(" | |\\/| | / _` | |  _| / _| / _ \\ | '  \\  |___|  | |  | ' \\  \\ V / / _` | (_-< | | / _ \\ | ' \\ \n");
    attron(COLOR_PAIR(ALIEN_COLOR_1));
    move(3, (COLS / 2) - 50);
    addstr(" |_|  |_| \\__,_|  \\__| \\__| \\___/ |_|_|_|       |___| |_||_|  \\_/  \\__,_| /__/ |_| \\___/ |_||_| \n");


    move(0, COLS - 19);
    attron(COLOR_PAIR(ALIEN_COLOR_4));
    addch('M');
    attron(COLOR_PAIR(NAVE_COLOR));
    addstr(" = MENU");
    move(0, COLS - 10);
    attron(COLOR_PAIR(ALIEN_COLOR_2));
    addch('Q');
    attron(COLOR_PAIR(NAVE_COLOR));
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

void *MOVER_DISPAROS()
{
    for (int i = 0; i < 3; i++)
    {
        if (!disparo[i].activo)
            continue;

        dispatch_semaphore_wait(sem_enemigo_disparo_bomba, DISPATCH_TIME_FOREVER);

        // Borrar el disparo en la posición anterior
        if (disparo[i].y < LINES - 2)
        {
            move(disparo[i].y + 1, disparo[i].x);
            addch(' ');
        }

        // Verificar colisiones con enemigos
        enemigo *current = enemy;
        while (current != NULL)
        {
            if (current->vivo && current->y == disparo[i].y && current->x == disparo[i].x)
            {
                // Actualizar estado después de la colisión
                playSoud_Impact();
                score += 20;
                current->vivo = 0;
                disparo[i].activo = 0;
                --Disparos_Actuales;
                --Enemigos_Actuales;
                move(current->py, current->px);
                addch(' ');
                refresh();
                Delete_enemy(current);
                break;
            }
            current = current->next;
        }

        // Si el disparo sigue activo después de la verificación de colisiones
        if (disparo[i].activo)
        {
            // Mover el disparo a la nueva posición
            move(disparo[i].y, disparo[i].x);
            addch(disparo[i].ch);
            disparo[i].y--;
        }
        else
        {
            // Si el disparo ya salió de la pantalla, eliminarlo
            move(disparo[i].y + 1, disparo[i].x);
            addch(' ');
        }

        // Si el disparo ya ha salido de la pantalla
        if (disparo[i].y <= 5)
        {
            disparo[i].activo = 0;
            --Disparos_Actuales;
            move(disparo[i].y + 1, disparo[i].x);
            addch(' ');
        }

        dispatch_semaphore_signal(sem_enemigo_disparo_bomba);
    }
}

void *MOVER_BOMBAS()
{
    // Mover bombas - Estrategia FCFS
    for (int i = 0; i < Max_BOMBAS; i++)
    {
        if (!bomba[i].activo)
            continue; // Saltar si la bomba no está activa

        dispatch_semaphore_wait(sem_enemigo_disparo_bomba, DISPATCH_TIME_FOREVER);

        if (bomba[i].y < LINES - 1) // Si la bomba no ha tocado el suelo
        {
            // Borrar la bomba en la posición anterior
            move(bomba[i].y - 1, bomba[i].x);
            addch(' ');

            // Mover la bomba a la nueva posición
            move(bomba[i].y, bomba[i].x);
            addch('o');

            // Incrementar la posición en Y para la próxima iteración
            bomba[i].y++;
        }
        else
        {
            // Si la bomba toca tierra, desactivarla y limpiar su rastro
            bomba[i].activo = 0;
            --Bombas_Actuales;
            move(bomba[i].y - 1, bomba[i].x);
            addch(' ');
        }

        dispatch_semaphore_signal(sem_enemigo_disparo_bomba);
    }
}

void *MOVER_ENEMIGOS()
{
    enemigo *current = enemy;
    int i = 0;

    while (current != NULL)
    {
        dispatch_semaphore_wait(sem_enemigo_disparo_bomba, DISPATCH_TIME_FOREVER);

        if (current->vivo == 1)
        {
            // Borrar la posición anterior del enemigo
            move(current->py, current->px);
            addch(' ');
            refresh();

            // Mover el cursor a la nueva posición
            move(current->y, current->x);

            // Cambiar el color del enemigo basado en el índice `i`
            if (i % 6 == 0)
                attron(COLOR_PAIR(ALIEN_COLOR_1));
            else if (i % 6 == 1)
                attron(COLOR_PAIR(ALIEN_COLOR_2));
            else if (i % 6 == 2)
                attron(COLOR_PAIR(ALIEN_COLOR_3));
            else if (i % 6 == 3)
                attron(COLOR_PAIR(ALIEN_COLOR_4));
            else if (i % 6 == 4)
                attron(COLOR_PAIR(ALIEN_COLOR_5));
            else if (i % 6 == 5)
                attron(COLOR_PAIR(ALIEN_COLOR_6));
            else
                attron(COLOR_PAIR(NAVE_COLOR));

            // Dibujar el enemigo en la nueva posición
            addch(current->ch);

            // Restablecer el color al predeterminado
            attron(COLOR_PAIR(NAVE_COLOR));

            // Actualizar las posiciones anteriores del enemigo
            current->py = current->y;
            current->px = current->x;

            // Verificar si el enemigo tiene que soltar una bomba
            int mi_random = 1 + (rand() % 100);
            if ((options.chanceBomba - mi_random) >= 0 && Bombas_Actuales < Max_BOMBAS)
            {
                for (int j = 0; j < Max_BOMBAS; j++)
                {
                    if (bomba[j].activo == 0)
                    {
                        // Activar una nueva bomba en la posición del enemigo
                        bomba[j].activo = 1;
                        Bombas_Actuales++;
                        bomba[j].y = current->y + 1;
                        bomba[j].x = current->x;
                        break;
                    }
                }
            }

            // Establecer la nueva posición del enemigo
            if (current->direccion == 0)
                current->x--;
            else if (current->direccion == 1)
                current->x++;

            // Verificar si el enemigo ha llegado a los límites de la pantalla
            if (current->x == COLS - 5)
            {
                current->y++;
                current->direccion = 0;
            }
            else if (current->x == 4)
            {
                current->y++;
                current->direccion = 1;
            }
        }
        dispatch_semaphore_signal(sem_enemigo_disparo_bomba);
        current = current->next;
        i++;
    }
    refresh();
}

void *VERIFICAR_END_GAME()
{
    // Verificar si quedan enemigos
    if (Enemigos_Actuales == 0)
    {
        playSoud_Win();
        win = 1;
        pthread_cancel(creacion_enemigo);
        pthread_cancel(mover_disparo_thread);
        pthread_cancel(mover_bomba_thread);
        pthread_cancel(mover_enemigo_thread);
    }

    // Verificar si alguna bomba del enemigo t dio
    for (int i = 0; i < Max_BOMBAS; i++)
    {
        if (bomba[i].y == nave.y && bomba[i].x == nave.x)
        {
            playSoud_GameOver();
            naveVida -= 1;
            if (naveVida == 2)
            {
                move(0, 27);
                addch(' ');
            }

            if (naveVida == 1)
            {
                move(0, 23);
                addch(' ');
            }

            if (naveVida == 0)
            {
                win = 0;
                pthread_cancel(creacion_enemigo);
                pthread_cancel(mover_disparo_thread);
                pthread_cancel(mover_bomba_thread);
                pthread_cancel(mover_enemigo_thread);
                break;
            }
        }
    }

    // Verificar si algun enemigo llego al final del mapa
    enemigo *current = enemy;
    while (current != NULL)
    {
        if (current->y == LINES - 1)
        {
            win = 0;
            break;
        }
        current = current->next;
    }
}

void *VERIFICAR_ENTRADA_MOV_NAVE()
{
    dispatch_semaphore_wait(sem_enemigo_disparo_bomba, DISPATCH_TIME_FOREVER);

    if (input == KEY_LEFT && nave.x > 4)
    {
        move(nave.y, nave.x);
        addch(' ');
        nave.x--;
    }
    else if (input == KEY_RIGHT && nave.x <= COLS - 5)
    {
        move(nave.y, nave.x);
        addch(' ');
        nave.x++;
    }

    else if (input == KEY_UP && nave.y > 6)
    {
        move(nave.y, nave.x);
        addch(' ');
        nave.y--;
    }
    else if (input == KEY_DOWN && nave.y <= LINES - 3)
    {
        move(nave.y, nave.x);
        addch(' ');
        nave.y++;
    }

    dispatch_semaphore_signal(sem_enemigo_disparo_bomba);
}

void *VERIFICAR_ENTRADA_DISPARAR()
{
    if (input == ' ' && Disparos_Actuales < 3)
    {
        playSoud_Shoot();
        for (int i = 0; i < 3; i++)
        {
            // Si el tienes disparos disponibles
            if (disparo[i].activo == 0)
            {
                disparo[i].activo = 1;
                Disparos_Actuales++;
                --score;
                // posicionar disparo sobre la nave
                disparo[i].y = nave.y - 1;
                disparo[i].x = nave.x;
                break;
            }
        }
    }
}

void *VERIFICAR_ENTRADA_OTROS(struct options options, int *win)
{
    if (input == 'q')
        *win = 2;
    else if (input == 'm')
        menu(&options);
}

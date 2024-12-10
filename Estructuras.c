#include<ncurses.h>

struct nave 
{
    int x,y;
    int vida;
    char ch;
};

typedef struct 
{
    int x;
    int y;
}Ship;






struct disparo
{
    int x,y;
    int activo; // 1 = activo 0 = inactivo
    char ch;
};

struct bomba
{
    int x,y;
    int activo; // lo mismo 
    char ch;
};

struct options
{
    int overall, enemigo;
    int disparos, bombas, chanceBomba; 
};

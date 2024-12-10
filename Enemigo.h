#include <stdbool.h>
#ifndef ENEMIGO_H_
#define ENEMIGO_H_

typedef struct enemigo
{
    int x,y;
    int px,py;
    int vivo; // 1 = vivo 0 = muerto 
    int direccion; //  0 = izq 1 = derecha
    char ch;
    struct enemigo *next;
}enemigo;

#endif
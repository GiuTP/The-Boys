#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"

void arrived (int t, struct heroi *h, struct base *b)
{
    int waiting;

    h->base = b->ID;

    if (b->present_heroes.num < b->capacity && b->waiting_queue.tamanho == 0)
        waiting = 1;
    else
    {
        if (h->patience > 10 * b->waiting_queue.tamanho)
            waiting = 1;
        else
            waiting = 0;
    }
    
    if (waiting)
    {
        wait (t, h, b);
        fprio_insere (h, h, t, t);
    }
    else
    {
        give_up (t, h, b);
        fprio_insere (h, h, t, t);
    }
}

int wait (int t, struct heroi *h, struct base *b)
{
    struct item_t *novo_h;

    if (!(novo_h = malloc (sizeof (struct item_t))))
        return -1;
    
    novo_h = h;
    
}

struct world my_world;
void travel (int t, struct heroi *h, struct base *d)
{
    
}

long aleat (long min, long max)
{
    return rand() % (max - min + 1) + min;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "eventos.h"

#define N_HABILIDADES 2 /* 10 */
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10

long aleat(long min, long max)
{
    return rand() % (max - min + 1) + min;
}

double distancia_pontos(struct coord p1, struct coord p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

struct event *cria_item(int tipo, int prio, struct hero *h, int base)
{
    struct event *item;

    if (!(item = malloc(sizeof(struct event))))
        return NULL;

    item->tipo = tipo;
    item->tempo = prio;
    item->h = h;
    item->base = base;

    return item;
}

void imprime_eventos(int tipo, int clock, struct event *ev, struct base *bases, int status)
{

    switch (tipo)
    {
    case 1:
        if (status == 1)
            printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n",
                   clock, ev->h->ID, ev->base, bases[ev->base].present_heroes->num, bases[ev->base].capacity);
        else
            printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n",
                   clock, ev->h->ID, ev->base, bases[ev->base].present_heroes->num, bases[ev->base].capacity);
        break;
    case 2:
        printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
               clock, ev->h->ID, ev->base, bases[ev->base].waiting_queue->tamanho - 1);
        break;
    case 3:
        printf("%6d: DESIST HEROI %2d BASE %d\n", clock, ev->h->ID, ev->base);
        break;
    case 4:
        printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ",
               clock, ev->base, bases[ev->base].present_heroes->num, bases[ev->base].present_heroes->cap);

        struct item_t *aux = bases[ev->base].waiting_queue->prim;
        while (aux != NULL)
        {
            printf("%2d ", aux->valor);
            aux = aux->prox;
        }
        printf("]\n");

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", clock, ev->base, ev->h->ID);
        break;
    case 5:
        printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
               clock, ev->h->ID, ev->base,
               bases[ev->base].present_heroes->num,
               bases[ev->base].capacity, ev->tpb);
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    }
}

int chega(int t, struct fprio_t **lef, struct hero *h, struct base b)
{
    struct event *evento;
    int wait;

    if (h->status == 0)
        return -1;

    h->base = b.ID;

    if ((b.present_heroes->num < b.capacity) && (b.waiting_queue->tamanho == 0))
        wait = 1;
    else if (h->patience > (10 * b.waiting_queue->tamanho))
        wait = 1;
    else
        wait = 0;

    if (wait)
    {
        evento = cria_item(ESPERA, t, h, b.ID);
        fprio_insere(*lef, evento, evento->tipo, evento->tempo);
        return 1;
    }
    else
    {
        evento = cria_item(DESISTE, t, h, b.ID);
        fprio_insere(*lef, evento, evento->tipo, evento->tempo);
        return 0;
    }
}

int espera(int t, struct fprio_t **lef, struct hero *h, struct base *b)
{
    struct event *evento;

    if (h->status == 0)
        return -1;

    if (!(evento = cria_item(AVISA, t, h, b->ID)))
        return 0;

    lista_insere(b->waiting_queue, h->ID, -1);
    fprio_insere(*lef, evento, evento->tipo, evento->tempo);

    return 1;
}

int desiste(int t, struct fprio_t **lef, struct hero h)
{
    struct event *evento;
    int new_base;

    if (h.status == 0)
        return -1;

    new_base = aleat(0, ((N_HABILIDADES * 5) / 5) - 1);
    if (!(evento = cria_item(VIAJA, t, &h, new_base)))
        return 0;

    fprio_insere(*lef, evento, evento->tipo, evento->tempo);

    return 1;
}

int avisa(int t, struct fprio_t **lef, struct hero *heroes, struct base *b)
{
    struct event *evento;
    struct hero *h;
    int heroID;

    while ((b->present_heroes->num < b->capacity) && b->waiting_queue->prim != NULL)
    {
        lista_retira(b->waiting_queue, &heroID, 0);
        h = &heroes[heroID];

        if (h->status != 0)
        {
            cjto_insere(b->present_heroes, heroID);
            if (!(evento = cria_item(ENTRA, t, h, b->ID)))
                return 0;

            fprio_insere(*lef, evento, evento->tipo, evento->tempo);
        }
    }

    return 1;
}

int entra(int t, struct fprio_t **lef, struct hero *h, struct base *b, int *tpb)
{
    struct event *evento;

    if (h->status == 0)
        return -1;

    *tpb = 15 + h->patience * aleat(1, 20);
    *tpb = *tpb + t;

    if (!(evento = cria_item(SAI, *tpb, h, b->ID)))
        return 0;

    fprio_insere(*lef, evento, evento->tipo, evento->tempo);

    return 1;
}

int sai(int t, struct fprio_t **lef, struct hero *h, struct base *b)
{
    struct event *evento;
    int new_base;

    if (h->status == 0)
        return -1;

    new_base = aleat(0, ((N_HABILIDADES * 5) / 5) - 1);
    cjto_retira(b->present_heroes, h->ID);

    if (!(evento = cria_item(VIAJA, t, h, new_base)))
        return 0;
    fprio_insere(*lef, evento, evento->tipo, evento->tempo);

    if (!(evento = cria_item(AVISA, t, h, b->ID)))
        return 0;

    fprio_insere(*lef, evento, evento->tipo, evento->tempo);

    return 1;
}

int morre(int t, struct fprio_t **lef, struct hero *h, struct base *b)
{
    struct event *evento;

    cjto_retira(b->present_heroes, h->ID);
    h->status = 0;

    if (!(evento = cria_item(AVISA, t, h, b->ID)))
        return 0;

    fprio_insere(*lef, evento, evento->tipo, evento->tempo);

    return 1;
}
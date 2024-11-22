#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "eventos.h"

#define N_HABILIDADES 2 /* 10 */
typedef enum
{
    CHEGA = 1,
    ESPERA,
    DESISTE,
    AVISA,
    ENTRA,
    SAI,
    VIAJA,
    MORRE,
    MISSAO,
    FIM
} EventType;

struct event *cria_evento(int t, int type, int h, int b)
{
    struct event *ev;

    if (!(ev = malloc(sizeof(struct event))))
        return NULL;

    ev->tempo = t;
    ev->tipo = type;
    ev->hero_id = h;
    ev->base_id = b;

    return ev;
}

void evento_chega(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int espera;

    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    h->base = b->ID;

    if ((b->present_heroes->num < b->capacity) && (b->waiting_queue->prim == NULL))
        espera = 1;
    else
        espera = (h->patience) > (10 * b->waiting_queue->tamanho);

    if (espera)
    {
        new_ev = cria_evento(ev->tempo, ESPERA, h->ID, b->ID);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n",
               ev->tempo, ev->hero_id, ev->base_id, b->present_heroes->num, b->capacity);
    }
    else
    {
        new_ev = cria_evento(ev->tempo, DESISTE, h->ID, b->ID);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n",
               ev->tempo, ev->hero_id, ev->base_id, b->present_heroes->num, b->capacity);
    }
}

void evento_espera(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;

    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    lista_insere(my_world->bases[ev->base_id].waiting_queue, ev->hero_id, -1);

    new_ev = cria_evento(ev->tempo, AVISA, h->ID, b->ID);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
           ev->tempo, h->ID, b->ID, b->waiting_queue->tamanho - 1);
}

void evento_desiste(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int new_base;

    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    new_base = aleat(0, my_world->total_bases - 1);

    new_ev = cria_evento(ev->tempo, VIAJA, h->ID, new_base);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: DESIST HEROI %2d BASE %d\n",
           ev->tempo, h->ID, b->ID);
}

void evento_avisa(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    int h;
    struct base *b;
    struct event *new_ev;
    struct item_t *aux;

    b = &my_world->bases[ev->base_id];

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ",
           ev->tempo, ev->base_id, b->present_heroes->num, b->capacity);
    aux = b->waiting_queue->prim;
    while (aux != NULL)
    {
        printf("%2d ", aux->valor);
        aux = aux->prox;
    }
    printf("]\n");

    while ((b->present_heroes->num < b->capacity) && (b->waiting_queue->prim != NULL))
    {
        lista_retira(b->waiting_queue, &h, 0);
        cjto_insere(b->present_heroes, h);

        new_ev = cria_evento(ev->tempo, ENTRA, h, b->ID);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->base_id, h);
        printf("Qtd. Heroes: %2d\n", b->present_heroes->num);
        printf("Herois presentes: ");
        cjto_imprime(b->present_heroes);
        printf("\n");
    }
}
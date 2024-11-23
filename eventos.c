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

struct event *cria_evento(int t, int type, int h, int b, int m)
{
    struct event *ev;

    if (!(ev = malloc(sizeof(struct event))))
        return NULL;

    ev->tempo = t;
    ev->tipo = type;
    ev->hero_id = h;
    ev->base_id = b;
    ev->mission_id = m;

    return ev;
}

int dist_pts(struct coord p1, struct coord p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

void swap(struct bases_m *a, struct bases_m *b)
{
    struct bases_m t = *a;
    *a = *b;
    *b = t;
}

int partition(struct bases_m *arr, int low, int high)
{
    int pivot = arr[high].dist;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].dist < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

void quick_sort(struct bases_m *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
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

    /* Verifica se o heroi esperara ou nao na fila de espera da base */
    if ((b->present_heroes->num < b->capacity) && (b->waiting_queue->prim == NULL))
        espera = 1;
    else
        espera = (h->patience) > (10 * b->waiting_queue->tamanho);

    /* Adicionando evento novo na LEF */
    if (espera)
    {
        new_ev = cria_evento(ev->tempo, ESPERA, h->ID, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n",
               ev->tempo, ev->hero_id, ev->base_id, b->present_heroes->num, b->capacity);
    }
    else
    {
        new_ev = cria_evento(ev->tempo, DESISTE, h->ID, b->ID, -1);
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

    new_ev = cria_evento(ev->tempo, AVISA, -1, b->ID, -1);
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

    /* Sorteito da nova base */
    new_base = aleat(0, my_world->total_bases - 1);

    new_ev = cria_evento(ev->tempo, VIAJA, h->ID, new_base, -1);
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

        new_ev = cria_evento(ev->tempo, ENTRA, h, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->base_id, h);
    }
}

void evento_entra(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int tpb;

    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    /* Calculo na permanencia do heroi */
    tpb = 15 + h->patience * aleat(1, 20);

    new_ev = cria_evento(ev->tempo + tpb, SAI, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
           ev->tempo, h->ID, b->ID, b->present_heroes->num, b->capacity, new_ev->tempo);
}

void evento_sai(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int new_base;

    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    cjto_retira(b->present_heroes, h->ID);
    new_base = aleat(0, my_world->total_bases - 1);

    new_ev = cria_evento(ev->tempo, VIAJA, h->ID, new_base, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    new_ev = cria_evento(ev->tempo, AVISA, -1, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
           ev->tempo, h->ID, b->ID, b->present_heroes->num, b->capacity);
}

void evento_viaja(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b_current, *b_next;
    struct event *new_ev;
    int dist, tempo;

    h = &my_world->heroes[ev->hero_id];
    b_current = &my_world->bases[h->base];
    b_next = &my_world->bases[ev->base_id];

    dist = dist_pts(b_current->locatization, b_next->locatization);
    tempo = dist / h->speed;

    new_ev = cria_evento(ev->tempo + tempo, CHEGA, h->ID, b_next->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
           ev->tempo, h->ID, b_current->ID, b_next->ID, dist, h->speed, new_ev->tempo);
}

void evento_morre(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct mission *m;
    struct event *new_ev;

    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];
    m = &my_world->missions[ev->mission_id];

    cjto_retira(b->present_heroes, h->ID);
    h->status = 0; /* Status de morto */

    new_ev = cria_evento(ev->tempo, AVISA, -1, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", ev->tempo, h->ID, m->ID);
}

void evento_missao(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct bases_m *bCand;
    struct cjto_t *union_skills, *us_temp;
    struct mission *m;
    struct event *new_ev;
    int i, j, bmp, risk;

    if (!(bCand = malloc(my_world->total_bases * sizeof(struct bases_m))))
        return;

    m = &my_world->missions[ev->mission_id];

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", ev->tempo, ev->mission_id, 0);
    cjto_imprime(m->skills_needed);
    printf(" ]\n");

    /* Insere no vetor bCand todas as bases do mundo */
    for (i = 0; i < my_world->total_bases; i++)
    {
        b = &my_world->bases[i];

        bCand[i].base_id = i;
        bCand[i].dist = dist_pts(m->localization, b->locatization);
        bCand[i].heroes_id = cjto_copia(b->present_heroes);

        union_skills = cjto_cria(N_HABILIDADES);
        for (j = 0; j < b->present_heroes->cap; j++)
        {
            if (cjto_pertence(b->present_heroes, j))
            {
                h = &my_world->heroes[j];
                us_temp = cjto_uniao(union_skills, h->skills);
                cjto_destroi(union_skills);
                union_skills = us_temp;
            }
        }
        bCand[i].Union = cjto_copia(union_skills);
        cjto_destroi(union_skills);
    }

    /* Ordena em ordem crescente de distancia as bases */
    quick_sort(bCand, 0, my_world->total_bases - 1);

    bmp = -1;
    for (i = 0; i < my_world->total_bases; i++)
    {
        b = &my_world->bases[bCand[i].base_id];

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", ev->tempo, m->ID, b->ID, bCand[i].dist);
        cjto_imprime(b->present_heroes);
        printf(" ]\n");

        for (j = 0; j < b->present_heroes->cap; j++)
        {
            if (cjto_pertence(b->present_heroes, j))
            {
                printf("%6d: MISSAO %d HAB HEROI %2d: [ ", ev->tempo, m->ID, j);
                cjto_imprime(my_world->heroes[j].skills);
                printf(" ]\n");
            }
        }

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", ev->tempo, m->ID, b->ID);
        cjto_imprime(bCand[i].Union);
        printf(" ]\n");

        if ((cjto_contem(bCand[i].Union, m->skills_needed)) && (bmp == -1))
            bmp = bCand[i].base_id;
    }

    /* bmp sera o primeiro indice depois de ordenar */
    if (bmp != -1)
    {
        b = &my_world->bases[bCand[0].base_id];

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", ev->tempo, m->ID, b->ID);
        cjto_imprime(bCand[bmp].Union);
        printf(" ]\n");

        for (i = 0; i < b->present_heroes->cap; i++)
        {
            if (cjto_pertence(b->present_heroes, i))
            {
                h = &my_world->heroes[i];
                risk = m->danger / (h->patience + h->experience + 1.0);

                if (risk > aleat(0, 30))
                {
                    new_ev = cria_evento(ev->tempo, MORRE, h->ID, b->ID, m->ID);
                    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);
                }
                else
                {
                    h->experience += 1;
                }
            }
        }
    }
    else
    {
        new_ev = cria_evento(ev->tempo + 24 * 60, MISSAO, -1, -1, ev->mission_id);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: MISSAO %d IMPOSSIVEL\n", ev->tempo, ev->mission_id);
    }

    for (i = 0; i < my_world->total_bases; i++)
    {
        cjto_destroi(bCand[i].heroes_id);
        cjto_destroi(bCand[i].Union);
    }
    free(bCand);
}

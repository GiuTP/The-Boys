// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "entidades.h"
#include "eventos.h"
#include "conjunto.h"

// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 10000  /* 525600 */
#define N_TAMANHO_MUNDO 10000 /* 20000 */
#define N_HABILIDADES 3       /* 10 */
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

long aleat(long min, long max)
{
    return rand() % (max - min + 1) + min;
}

struct world mundo_inicia()
{
    struct world mundo;

    mundo.clock = T_INICIO;
    mundo.size_world.x = mundo.size_world.y = N_TAMANHO_MUNDO;
    mundo.total_skills = N_HABILIDADES;
    mundo.total_heroes = N_HABILIDADES * 5;
    mundo.total_bases = mundo.total_heroes / 5;
    mundo.total_missions = T_FIM_DO_MUNDO / 100;

    return mundo;
}

struct hero *herois_inicia(struct world *my_world)
{
    int i;

    if (!(my_world->heroes = malloc(my_world->total_heroes * sizeof(struct hero))))
        return NULL;

    for (i = 0; i < my_world->total_heroes; i++)
    {
        my_world->heroes[i].ID = i;
        my_world->heroes[i].status = 1;
        my_world->heroes[i].experience = 0;
        my_world->heroes[i].patience = aleat(0, 100);
        my_world->heroes[i].speed = aleat(50, 5000);
    }

    return my_world->heroes;
}

struct base *bases_inicia(struct world *my_world)
{
    int i;

    if (!(my_world->bases = malloc(my_world->total_bases * sizeof(struct base))))
        return NULL;

    for (i = 0; i < my_world->total_bases; i++)
    {
        my_world->bases[i].ID = i;
        my_world->bases[i].locatization.x = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->bases[i].locatization.y = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->bases[i].capacity = aleat(3, 10);
        my_world->bases[i].present_heroes = cjto_cria(my_world->total_heroes);
        my_world->bases[i].waiting_queue = lista_cria();
    }

    return my_world->bases;
}

void heroes_evi(struct fprio_t **lef, struct world *my_world)
{
    struct event *ev;
    int i, tempo;

    for (i = 0; i < my_world->total_heroes; i++)
    {
        my_world->heroes[i].base = aleat(0, my_world->total_bases - 1);
        tempo = aleat(0, 4320);
        ev = cria_evento(tempo, CHEGA, my_world->heroes[i].ID, my_world->heroes[i].base);
        fprio_insere(*lef, ev, ev->tipo, ev->tempo);
    }
}

void end_evi(struct fprio_t **lef)
{
    struct event *ev;

    ev = cria_evento(T_FIM_DO_MUNDO, FIM, -1, -1);
    fprio_insere(*lef, ev, ev->tipo, ev->tempo);
}

void mundo_destroi(struct world *my_world)
{
    int i;

    for (i = 0; i < my_world->total_bases; i++)
    {
        cjto_destroi(my_world->bases[i].present_heroes);
        lista_destroi(my_world->bases[i].waiting_queue);
    }

    free(my_world->heroes);
    free(my_world->bases);
}
// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "entidades.h"
#include "eventos.h"
#include "conjunto.h"

// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 5000  /* 525600 */
#define N_TAMANHO_MUNDO 8000 /* 20000 */
#define N_HABILIDADES 3      /* 10 */
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
        my_world->heroes[i].speed = aleat(1, 10);
        my_world->heroes[i].skills = cjto_aleat(aleat(1, 2), N_HABILIDADES);
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

struct mission *missoes_inicia(struct world *my_world)
{
    int i;

    if (!(my_world->missions = malloc(my_world->total_missions * sizeof(struct mission))))
        return NULL;

    for (i = 0; i < my_world->total_missions; i++)
    {
        my_world->missions[i].ID = i;
        my_world->missions[i].localization.x = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->missions[i].localization.y = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->missions[i].skills_needed = cjto_aleat(aleat(1, 3), my_world->total_skills);
        my_world->missions[i].danger = aleat(0, 100);
    }

    return my_world->missions;
}

void heroes_evi(struct fprio_t **lef, struct world *my_world)
{
    struct event *ev;
    int i, tempo;

    for (i = 0; i < my_world->total_heroes; i++)
    {
        my_world->heroes[i].base = aleat(0, my_world->total_bases - 1);
        tempo = aleat(0, 4320);
        ev = cria_evento(tempo, CHEGA, my_world->heroes[i].ID, my_world->heroes[i].base, -1);
        fprio_insere(*lef, ev, ev->tipo, ev->tempo);
    }
}

void mission_evi(struct fprio_t **lef, struct world *my_world)
{
    struct event *ev;
    int i, tempo;

    for (i = 0; i < my_world->total_missions; i++)
    {
        tempo = aleat(0, T_FIM_DO_MUNDO);
        ev = cria_evento(tempo, MISSAO, -1, -1, my_world->missions[i].ID);
        fprio_insere(*lef, ev, ev->tipo, ev->tempo);
    }
}

void end_evi(struct fprio_t **lef)
{
    struct event *ev;

    ev = cria_evento(T_FIM_DO_MUNDO, FIM, -1, -1, -1);
    fprio_insere(*lef, ev, ev->tipo, ev->tempo);
}

void mundo_destroi(struct world *my_world)
{
    int i;

    /* Desaloca a memoria alocada para o conjunto de habilidades de cada heroi */
    for (i = 0; i < my_world->total_heroes; i++)
        cjto_destroi(my_world->heroes[i].skills);

    /* Desaloca a memoria alocada para cada base nos casos:
     * - Conjunto de herois presentes na base
     * - Fila de espera da base */
    for (i = 0; i < my_world->total_bases; i++)
    {
        cjto_destroi(my_world->bases[i].present_heroes);
        lista_destroi(my_world->bases[i].waiting_queue);
    }

    /* Desaloca a memoria alocada para o conjunto de habilidades necessarias de cada missao */
    for (i = 0; i < my_world->total_missions; i++)
        cjto_destroi(my_world->missions[i].skills_needed);

    free(my_world->heroes);
    free(my_world->bases);
    free(my_world->missions);
}
#include <stdlib.h>
#include "fprio.h"
#include "entidades.h"
#include "eventos.h"
#include "conjunto.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
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
    struct world worldi;

    worldi.clock = T_INICIO;
    worldi.tam_mundo.x = N_TAMANHO_MUNDO;
    worldi.tam_mundo.y = N_TAMANHO_MUNDO;
    worldi.num_habilidades = N_HABILIDADES;
    worldi.num_herois = N_HABILIDADES * 5;
    worldi.num_bases = worldi.num_herois / 5;
    worldi.num_missões = T_FIM_DO_MUNDO / 100;

    return worldi;
}

struct hero *herois_inicia(struct world *my_world)
{
    int i;

    if (!(my_world->herois = malloc(my_world->num_herois * sizeof(struct hero))))
        return NULL;

    for (i = 0; i < my_world->num_herois; i++)
    {
        my_world->herois[i].ID = i;
        my_world->herois[i].status = 1;
        my_world->herois[i].xp = 0;
        my_world->herois[i].paciência = aleat(0, 100);
        my_world->herois[i].velocidade = aleat(50, 5000);
        my_world->herois[i].habilidades = cjto_aleat(aleat(1, 3), N_HABILIDADES);
    }

    return my_world->herois;
}

struct base *bases_inicia(struct world *my_world)
{
    int i;

    if (!(my_world->bases = malloc(my_world->num_bases * sizeof(struct base))))
        return NULL;

    for (i = 0; i < my_world->num_bases; i++)
    {
        my_world->bases[i].ID = i;
        my_world->bases[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->bases[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->bases[i].lotação = aleat(3, 10);
        my_world->bases[i].h_presentes = cjto_cria(my_world->num_herois);
        my_world->bases[i].f_espera = fila_cria();
        my_world->bases[i].f_max = 0;
        my_world->bases[i].part_missão = 0;
    }

    return my_world->bases;
}

struct mission *missoes_inicia(struct world *my_world)
{
    int i;

    if (!(my_world->missões = malloc(my_world->num_missões * sizeof(struct mission))))
        return NULL;

    for (i = 0; i < my_world->num_missões; i++)
    {
        my_world->missões[i].ID = i;
        my_world->missões[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->missões[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
        my_world->missões[i].hab_necessarias = cjto_aleat(aleat(6, 10), my_world->num_habilidades);
        my_world->missões[i].perigo = aleat(0, 100);
        my_world->missões[i].tent = 1;
    }

    return my_world->missões;
}

struct statistics estatisticas_inicia(struct world *my_world)
{
    my_world->infos.ev_tratados = 0;
    my_world->infos.tent_max_missão = 0;
    my_world->infos.tent_min_missão = __INT_MAX__;
    my_world->infos.comp_missões = 0;
    my_world->infos.total_mortes = 0;

    return my_world->infos;
}

void heroes_evi(struct fprio_t **lef, struct world *my_world)
{
    struct event *ev;
    int i, t;

    for (i = 0; i < my_world->num_herois; i++)
    {
        my_world->herois[i].base = aleat(0, my_world->num_bases - 1);
        t = aleat(0, 60 * 24 * 3);
        ev = cria_evento(t, CHEGA, my_world->herois[i].ID, my_world->herois[i].base, -1);
        fprio_insere(*lef, ev, ev->tipo, ev->tempo);
    }

    return;
}

void mission_evi(struct fprio_t **lef, struct world *my_world)
{
    struct event *ev;
    int i, t;

    for (i = 0; i < my_world->num_missões; i++)
    {
        t = aleat(0, T_FIM_DO_MUNDO);
        ev = cria_evento(t, MISSAO, -1, -1, my_world->missões[i].ID);
        fprio_insere(*lef, ev, ev->tipo, ev->tempo);
    }

    return;
}

void end_evi(struct fprio_t **lef)
{
    struct event *ev;

    ev = cria_evento(T_FIM_DO_MUNDO, FIM, -1, -1, -1);
    fprio_insere(*lef, ev, ev->tipo, ev->tempo);

    return;
}

void mundo_destroi(struct world *my_world)
{
    int i;

    /* Libera a memoria alocada para o conjunto de habilidades de cada heroi */
    for (i = 0; i < my_world->num_herois; i++)
        cjto_destroi(my_world->herois[i].habilidades);

    /* Libera a memoria alocada para os conjuntos de heroi e fila de cada base */
    for (i = 0; i < my_world->num_bases; i++)
    {
        cjto_destroi(my_world->bases[i].h_presentes);
        fila_destroi(my_world->bases[i].f_espera);
    }

    /* Libera a memoria alocada para o conjunto de habilidades necessarias de cada missao */
    for (i = 0; i < my_world->num_missões; i++)
        cjto_destroi(my_world->missões[i].hab_necessarias);

    /* Libera a memoria alocada para os vetores dos elementos do mundo */
    free(my_world->herois);
    free(my_world->bases);
    free(my_world->missões);

    return;
}
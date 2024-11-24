// programa principal do projeto "The Boys - 2024/2"
// Autor: Giuliano Thiago Pinherio Tavares, GRR 20240305

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
#include "eventos.h"
#include "entidades.h"

// seus #defines vão aqui
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

// minimize o uso de variáveis globais

// programa principal
int main()
{
    struct world my_world;
    struct event *ev;
    struct fprio_t *lef;
    int tipo, tempo;

    srand(0);

    /* Inicializa todas as entidades e campos do mundo */
    my_world = mundo_inicia();
    my_world.heroes = herois_inicia(&my_world);
    my_world.bases = bases_inicia(&my_world);
    my_world.infos = estatisticas_inicia(&my_world);
    my_world.missions = missoes_inicia(&my_world);

    /* Inicializa a LEF */
    lef = fprio_cria();

    /* Cria os eventos iniciais do mundo */
    heroes_evi(&lef, &my_world);
    mission_evi(&lef, &my_world);
    end_evi(&lef);

    /* Loop principal da simulacao */
    do
    {
        ev = fprio_retira(lef, &tipo, &tempo);
        my_world.clock = tempo;

        switch (tipo)
        {
        case CHEGA:
            if (my_world.heroes[ev->hero_id].status)
                evento_chega(&lef, &my_world, ev);
            break;
        case ESPERA:
            if (my_world.heroes[ev->hero_id].status)
                evento_espera(&lef, &my_world, ev);
            break;
        case DESISTE:
            if (my_world.heroes[ev->hero_id].status)
                evento_desiste(&lef, &my_world, ev);
            break;
        case AVISA:
            if (my_world.heroes[ev->hero_id].status)
                evento_avisa(&lef, &my_world, ev);
            break;
        case ENTRA:
            if (my_world.heroes[ev->hero_id].status)
                evento_entra(&lef, &my_world, ev);
            break;
        case SAI:
            if (my_world.heroes[ev->hero_id].status)
                evento_sai(&lef, &my_world, ev);
            break;
        case VIAJA:
            if (my_world.heroes[ev->hero_id].status)
                evento_viaja(&lef, &my_world, ev);
            break;
        case MORRE:
            evento_morre(&lef, &my_world, ev);
            break;
        case MISSAO:
            evento_missao(&lef, &my_world, ev);
            break;
        case FIM:
            evento_fim(my_world, ev);
            break;
        }
        free(ev);
        ev = NULL;

    } while (tipo != FIM);

    /* Libera toda memoria alocada para a simulacao */
    mundo_destroi(&my_world);
    fprio_destroi(lef);

    return 0;
}

// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
#include "eventos.h"
#include "entidades.h"

// seus #defines vão aqui
#define T_FIM_DO_MUNDO 8000 /* 525600 */
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

    my_world = mundo_inicia();
    my_world.heroes = herois_inicia(&my_world);
    my_world.bases = bases_inicia(&my_world);

    lef = fprio_cria();

    heroes_evi(&lef, &my_world);
    end_evi(&lef);

    /* Semente randômica */
    srand(0);

    do
    {
        ev = fprio_retira(lef, &tipo, &tempo);
        my_world.clock = tempo;

        switch (tipo)
        {
        case CHEGA:
            evento_chega(&lef, &my_world, ev);
            break;
        case ESPERA:
            evento_espera(&lef, &my_world, ev);
            break;
        case DESISTE:
            evento_desiste(&lef, &my_world, ev);
            break;
        case AVISA:
            evento_avisa(&lef, &my_world, ev);
            break;
        case ENTRA:
            evento_entra(&lef, &my_world, ev);
            break;
        case SAI:
            evento_sai(&lef, &my_world, ev);
            break;
        case FIM:
            printf("Fim do mundo\n");
            break;
        }
        free(ev);
        ev = NULL;

    } while (tipo != FIM);

    /* libera a memoria alocada */
    // destruir o mundo
    mundo_destroi(&my_world);
    fprio_destroi(lef);

    return 0;
}

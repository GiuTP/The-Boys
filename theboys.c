// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
#include "eventos.h"
#include "entidades.h"

// seus #defines vão aqui
#define T_FIM_DO_MUNDO 300000 /* 525600 */
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 2 /* 10 */
#define MIN_SKILL 1
#define MAX_SKILL 3
#define MIN_PACIENCIA 0
#define MAX_PACIENCIA 100
#define MIN_VELOCIDADE 50
#define MAX_VELOCIDADE 5000
#define MIN_CAPACIDADE 3
#define MAX_CAPACIDADE 10
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

// minimize o uso de variáveis globais

// programa principal
int main()
{
    /* Variaveis usadas */
    struct world my_world;
    struct hero *heroes;
    struct base *bases;
    struct fprio_t *lef;
    struct event *ev, *ev_rem;
    int tipo, tempo;
    int i, time;

    /* Semente randômica */
    srand(0);

    /* ------- Inicializa o mundo ------- */
    my_world.clock = 0;
    my_world.size_world.x = my_world.size_world.y = N_TAMANHO_MUNDO;
    my_world.total_skills = N_HABILIDADES;
    my_world.total_heroes = my_world.total_skills * 5;
    my_world.total_bases = my_world.total_heroes / 5;
    my_world.total_missions = T_FIM_DO_MUNDO / 100;

    /* ------- Inicializa o vetor de herois ------- */
    if (!(heroes = malloc(my_world.total_heroes * sizeof(struct hero))))
        return -1;

    for (i = 0; i < my_world.total_heroes; i++)
    {
        heroes[i].ID = i;
        heroes[i].status = 1;
        heroes[i].experience = 0;
        heroes[i].patience = aleat(MIN_PACIENCIA, MAX_PACIENCIA);
        heroes[i].speed = aleat(MIN_VELOCIDADE, MAX_VELOCIDADE);
        if (!(heroes[i].skills = cjto_aleat(aleat(MIN_SKILL, MAX_SKILL), N_HABILIDADES)))
            return -1;
    }

    /* ------- Inicializa o vetor das bases ------- */
    if (!(bases = malloc(my_world.total_bases * sizeof(struct base))))
        return -1;

    for (i = 0; i < my_world.total_bases; i++)
    {
        bases[i].ID = i;
        bases[i].capacity = aleat(MIN_CAPACIDADE, MAX_CAPACIDADE);
        bases[i].locatization.x = aleat(0, N_TAMANHO_MUNDO - 1);
        bases[i].locatization.y = aleat(0, N_TAMANHO_MUNDO - 1);
        if (!(bases[i].present_heroes = cjto_cria(bases[i].capacity)))
            return -1;
        if (!(bases[i].waiting_queue = lista_cria()))
            return -1;
    }

    /* ------- Inicializa a LEF ------- */
    if (!(lef = fprio_cria()))
        return -1;

    /* ------- Inicializa os eventos inicias ------- */
    /* herois */
    for (i = 0; i < my_world.total_heroes; i++)
    {
        heroes[i].base = aleat(0, my_world.total_bases - 1);
        time = aleat(0, 4320);
        ev = cria_item(CHEGA, time, &heroes[i], heroes[i].base);
        fprio_insere(lef, ev, ev->tipo, ev->tempo);
    }
    /* missoes */

    /* fim do mundo */
    time = T_FIM_DO_MUNDO;
    ev = cria_item(FIM, time, NULL, -1);
    fprio_insere(lef, ev, ev->tipo, ev->tempo);

    int status;
    // executar o laço de simulação
    do
    {
        ev_rem = fprio_retira(lef, &tipo, &tempo);

        my_world.clock = ev_rem->tempo;
        switch (ev_rem->tipo)
        {
        case CHEGA:
            status = chega(tempo, &lef, &heroes[ev_rem->h->ID], bases[ev_rem->base]);
            imprime_eventos(CHEGA, my_world.clock, ev_rem, bases, status);
            break;
        case ESPERA:
            espera(tempo, &lef, &heroes[ev_rem->h->ID], &bases[ev_rem->base]);
            imprime_eventos(ESPERA, my_world.clock, ev_rem, bases, status);
            break;
        case DESISTE:
            status = desiste(tempo, &lef, heroes[ev_rem->h->ID]);
            imprime_eventos(DESISTE, my_world.clock, ev_rem, bases, status);
            break;
        case AVISA:
            status = avisa(tempo, &lef, heroes, &bases[ev_rem->base]);
            imprime_eventos(AVISA, my_world.clock, ev_rem, bases, status);
            break;
        case ENTRA:
            entra(tempo, &lef, &heroes[ev_rem->h->ID], &bases[ev_rem->base], &ev_rem->tpb);
            imprime_eventos(ENTRA, my_world.clock, ev_rem, bases, status);
            break;
        case SAI:
            sai(tempo, &lef, &heroes[ev_rem->h->ID], &bases[ev_rem->base]);
            printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
                   my_world.clock, ev_rem->h->ID, ev_rem->base,
                   bases[ev_rem->base].present_heroes->num,
                   bases[ev_rem->base].capacity);
            break;
        case VIAJA:

            break;
        case MORRE:
            morre(tempo, &lef, &heroes[ev_rem->h->ID], &bases[ev_rem->base]);
            printf("%6d: MORRE  HEROI %2d MISSAO %d", my_world.clock, ev_rem->h->ID, ev_rem->base); /* Mudar pra missao */
            break;
        case MISSAO:

            break;
        default:
            printf("%6d: FIM\n", my_world.clock);
            break;
        }
        free(ev_rem);
        ev_rem = NULL;
    } while (my_world.clock < T_FIM_DO_MUNDO);

    /* libera a memoria alocada */
    // destruir o mundo
    for (i = 0; i < my_world.total_bases; i++)
    {
        cjto_destroi(bases[i].present_heroes);
        lista_destroi(bases[i].waiting_queue);
    }

    for (i = 0; i < my_world.total_heroes; i++)
        cjto_destroi(heroes[i].skills);

    free(heroes);
    free(bases);

    fprio_destroi(lef);

    return 0;
}

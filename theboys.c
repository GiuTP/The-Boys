// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"

// seus #defines vão aqui
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10


// minimize o uso de variáveis globais

// programa principal
int main ()
{
    struct world my_world;
    struct heroi *heroes;
    struct base *bases;
    int i;

    /* Inicializa o mundo */
    my_world.clock = 0;
    my_world.size_world.x = my_world.size_world.y = N_TAMANHO_MUNDO;
    my_world.total_skills = N_HABILIDADES;
    my_world.total_heroes = my_world.total_skills * 5;
    my_world.total_bases = my_world.total_heroes / 5;
    my_world.total_missions = T_FIM_DO_MUNDO/100;

    if (!(heroes = malloc (50 * sizeof (struct heroi))))
        return -1;

    /* Inicializa os herois */
    for (i = 0; i < my_world.total_heroes; i++)
    {
        heroes[i].ID = i;
        heroes[i].experience = 0;
        heroes[i].patience = aleat(0, 100);
        heroes[i].speed = aleat(50, 5000);
        heroes[i].skill.cap = ateat(1, 3);
    }

    /* Inicializa as bases */
    for (i = 0; i < my_world.total_bases; i++)
    {
        bases[i].ID = i;
        bases[i].locatization.x = aleat(0, N_TAMANHO_MUNDO - 1);
        bases[i].locatization.y = aleat(0, N_TAMANHO_MUNDO - 1);
        bases[i].capacity = aleat(3, 10);
        bases[i].present_heroes = cjto_cria(bases[i].capacity);
        bases[i].waiting_queue = lista_cria();
    }
    
    // executar o laço de simulação
    while (my_world.clock < T_FIM_DO_MUNDO)
    // destruir o mundo

    return 0;
}


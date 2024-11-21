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
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
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
    struct fprio_t *lef;

    my_world.clock = T_INICIO;
    my_world.size_world.x = my_world.size_world.y = N_TAMANHO_MUNDO;
    my_world.total_skills = N_HABILIDADES;
    my_world.total_heroes = N_HABILIDADES * 5;
    my_world.total_heroes = my_world.total_heroes / 5;
    my_world.total_missions = T_FIM_DO_MUNDO / 100;

    /* Semente randômica */
    srand(0);

    /* ------- Inicializa o mundo ------- */

    /* ------- Inicializa a LEF ------- */

    /* ------- Inicializa os eventos inicias ------- */
    /* herois */

    /* missoes */

    /* fim do mundo */

    // executar o laço de simulação

    /* libera a memoria alocada */
    // destruir o mundo

    return 0;
}

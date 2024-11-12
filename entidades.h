#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "lista.h"

/* Estrutura de um heroi */
struct heroi 
{
    int ID; /* identicador do heroi */
    struct cjto_t skill; /* conjunto de habilidades */
    int patience; /* paciencia do heroi */
    int speed; /* velocidade do heroi */
    int experience; /* experiencia do heroi */
    int base; /* base onde o heroi esta */
};

struct coord
{
    int x;
    int y;
};

/* Estrutura de uma base */
struct base
{
    int ID; /* identicador da base */
    int capacity; /* capacidade da base */
    struct cjto_t *present_heroes; /* conjunto de herois presentes */
    struct lista_t *waiting_queue; /* fila onde os herois esperam */
    struct coord locatization; /* localizacao da base */
};

/* Estrutura de uma missao */
struct mission
{
    int ID; /* identicador da missao */
    struct cjto_t skills_needed; /* conjunto de habilidades necessarias */
    int danger; /* nivel de perigo da missao */
    struct coord localization; /* localizacao da missao */
};

/* Estrutura de um mundo */
struct world
{
    int total_heroes; /* numero de herois no mundo */
    struct heroi *heroes; /* vetor de herois */
    int total_bases; /* numero de bases no mundo */
    struct base *bases; /* vetor de bases */
    int total_missions; /* numero de missoes no mundo */
    struct mission *missions; /* vetor de missoes */
    int total_skills; /* numero de habilidades distintas no mundo */
    struct coord size_world; /* tamanho do mundo */
    int clock; /* tempo do mundo */
};

#endif 
#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "lista.h"

/* Estrutura de um heroi */
struct hero
{
    int ID;                /* identicador do heroi */
    int status;            /* status do heroi */
    struct cjto_t *skills; /* conjunto de habilidades */
    int patience;          /* paciencia do heroi */
    int speed;             /* velocidade do heroi */
    int experience;        /* experiencia do heroi */
    int base;              /* base onde o heroi esta */
};

/* Coordenada das bases */
struct coord
{
    int x;
    int y;
};

/* Estrutura de uma base */
struct base
{
    int ID;                        /* identicador da base */
    int capacity;                  /* capacidade da base */
    struct cjto_t *present_heroes; /* conjunto de herois presentes da base */
    struct lista_t *waiting_queue; /* fila de espera da base */
    struct coord locatization;     /* localizacao da base */
};

/* Estrutura de uma missao */
struct mission
{
    int ID;                      /* identicador da missao */
    struct cjto_t skills_needed; /* conjunto de habilidades necessarias */
    int danger;                  /* nivel de perigo da missao */
    struct coord localization;   /* localizacao da missao */
};

/* Estrutura de um mundo */
struct world
{
    int total_heroes;        /* numero de herois no mundo */
    struct hero *heroes;     /* vetor de herois */
    int total_bases;         /* numero de bases no mundo */
    struct base *bases;      /* vetor de bases */
    int total_missions;      /* numero de missoes no mundo */
    struct mission missions; /* vetor de missoes */
    int total_skills;        /* numero de habilidades distintas no mundo */
    struct coord size_world; /* tamanho do mundo */
    int clock;               /* tempo do mundo */
};

struct event
{
    int tipo;       /* tipo do evento [1..10] */
    int tempo;      /* instante que o evento ocorre */
    int base;       /* base envolvida no evento */
    struct hero *h; /* heroi envolvido no evento */
    int tpb;
};

#endif
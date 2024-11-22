#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "lista.h"

/* -------------------- Entidades do mundo -------------------- */

/* Heroi */
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

/* Bases */
struct base
{
    int ID;                        /* identicador da base */
    int capacity;                  /* capacidade da base */
    struct cjto_t *present_heroes; /* conjunto de herois presentes da base */
    struct lista_t *waiting_queue; /* fila de espera da base */
    struct coord locatization;     /* localizacao da base */
};

/* Missao */
struct mission
{
    int ID;                      /* identicador da missao */
    struct cjto_t skills_needed; /* conjunto de habilidades necessarias */
    int danger;                  /* nivel de perigo da missao */
    struct coord localization;   /* localizacao da missao */
};

/* Mundo */
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

/* Eventos */
struct event
{
    int tipo;
    int tempo;
    int hero_id;
    int base_id;
};

/* -------------------- Inicializacao do mundo -------------------- */

long aleat(long min, long max);

/* Inicializa os campos basicos do mundo */
/* Retorno: uma struct world */
struct world mundo_inicia();

/* Inicializa o vetor de herois do mundo. */
/* Retorno: ponteiro para struct hero ou NULL em caso de erro */
struct hero *herois_inicia(struct world *my_world);

/* Inicializa o vetor de bases do mundo. */
/* Retorno: ponteiro para struct base ou NULL em caso de erro */
struct base *bases_inicia(struct world *my_world);

/* ----------------- Inicializacao dos eventos iniciais ----------------- */

void heroes_evi(struct fprio_t **lef, struct world *my_world);

void end_evi(struct fprio_t **lef);

/* -------------------- Destroi o mundo -------------------- */

/* Destroi os elementos dinamicos do mundo, liberando a memoria alocada para cada um. */
void mundo_destroi(struct world *my_world);

#endif
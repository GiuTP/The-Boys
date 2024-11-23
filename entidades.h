#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "lista.h"

/* -------------------- Entidades do mundo -------------------- */

/* Descreve atributos de um heroi */
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

/* Descreve atributos de uma base */
struct base
{
    int ID;                        /* identicador da base */
    int capacity;                  /* capacidade da base */
    struct cjto_t *present_heroes; /* conjunto de herois presentes da base */
    struct lista_t *waiting_queue; /* fila de espera da base */
    struct coord locatization;     /* localizacao da base */
};

/* Descreve atributos de uma missao */
struct mission
{
    int ID;                       /* identicador da missao */
    struct cjto_t *skills_needed; /* conjunto de habilidades necessarias */
    int danger;                   /* nivel de perigo da missao */
    struct coord localization;    /* localizacao da missao */
};

/* Descreve atributos de um mundo */
struct world
{
    int total_heroes;         /* numero de herois no mundo */
    struct hero *heroes;      /* vetor de herois */
    int total_bases;          /* numero de bases no mundo */
    struct base *bases;       /* vetor de bases */
    int total_missions;       /* numero de missoes no mundo */
    struct mission *missions; /* vetor de missoes */
    int total_skills;         /* numero de habilidades distintas no mundo */
    struct coord size_world;  /* tamanho do mundo */
    int clock;                /* tempo do mundo */
};

/* Descreve atributos vinculados a um evento */
struct event
{
    int tipo;
    int tempo;
    int hero_id;
    int base_id;
    int mission_id;
};

struct bases_m
{
    int base_id;
    int dist;
    struct cjto_t *heroes_id;
    struct cjto_t *Union;
};

struct statistics
{
    /* Estatisticas relacionadas as bases */
    int max_pres_h;       /* Qtd. maxima de herois na base */
    int max_queue;        /* Qtd. maxima de herois na fila da base */
    int complete_mission; /* Qtd. de missoes que a base participou */
};

/* -------------------- Inicializacao do mundo -------------------- */

/* Retorna um novo aleatorio entre min e max */
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

/* Inicializa o vetor de missoes do mundo. */
/* Retorno: ponteiro para struct mission ou NULL em caso de erro. */
struct mission *missoes_inicia(struct world *my_world);

/* ----------------- Inicializacao dos eventos iniciais ----------------- */

/* Inicializa os eventos CHEGA iniciais de cada heroi do mundo. */
/* Cria e adiciona na LEF o evento CHEGA para cada hero. */
void heroes_evi(struct fprio_t **lef, struct world *my_world);

/* Inicializa todas as missoes do mundo em um tempo aleatorio t. */
/* Cria e adiciona na LEF o evento MISSAO para cada ID de missao. */
void mission_evi(struct fprio_t **lef, struct world *my_world);

/* Cria e adiciona na LEF o evento FIM (fim do mundo) */
void end_evi(struct fprio_t **lef);

/* -------------------- Destroi o mundo -------------------- */

/* Destroi os elementos dinamicos do mundo, liberando a memoria alocada para cada um. */
void mundo_destroi(struct world *my_world);

#endif
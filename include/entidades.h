#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "fila.h"

/* -------------------- Entidades do mundo -------------------- */

/* Descreve atributos de um heroi */
struct hero
{
    int ID;                     /* identicador do heroi */
    int status;                 /* status do heroi */
    struct cjto_t *habilidades; /* conjunto de habilidades */
    int paciência;              /* paciencia do heroi */
    int velocidade;             /* velocidade do heroi */
    int xp;                     /* experiencia do heroi */
    int base;                   /* base onde o heroi esta */
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
    int ID;                     /* identicador da base */
    int lotação;                /* capacidade da base */
    struct cjto_t *h_presentes; /* conjunto de herois presentes da base */
    struct fila_t *f_espera;    /* fila de espera da base */
    struct coord local;         /* localizacao da base */
    int f_max;                  /* fila maxima usada nas estatisticas*/
    int part_missão;            /* Qtd. de missoes participadas */
};

/* Descreve atributos de uma missao */
struct mission
{
    int ID;                         /* identicador da missao */
    struct cjto_t *hab_necessarias; /* conjunto de habilidades necessarias */
    int perigo;                     /* nivel de perigo da missao */
    struct coord local;             /* localizacao da missao */
    int tent;                       /* qtd. de tentativas da missao */
};

/* Descreve dados usados quando o mundo acaba */
struct statistics
{
    int ev_tratados;     /* acumulador de eventos tratatos na simulacao */
    int comp_missões;    /* acumulador de missoes completadas na simulacao */
    int tent_min_missão; /* usado para ver tentativa minima nas missoes */
    int tent_max_missão; /* usado para ver tentativa maxima nas missoes */
    int total_mortes;    /* total de mortes na simulacao */
};

/* Descreve atributos de um mundo */
struct world
{
    int num_herois;          /* numero de herois no mundo */
    struct hero *herois;     /* vetor de herois */
    int num_bases;           /* numero de bases no mundo */
    struct base *bases;      /* vetor de bases */
    int num_missões;         /* numero de missoes no mundo */
    struct mission *missões; /* vetor de missoes */
    int num_habilidades;     /* numero de habilidades distintas no mundo */
    struct coord tam_mundo;  /* tamanho do mundo */
    int clock;               /* tempo do mundo */
    struct statistics infos; /* estatisticas do mundo */
};

/* Descreve atributos vinculados a um evento */
struct event
{
    int tipo;  /* tipo do evento */
    int tempo; /* hora que o evento ocorrera */
    int h_id;  /* heroi vinculado ao evento */
    int b_id;  /* base vinculada ao evento */
    int m_id;  /* missao vinculada ao evento */
};

/* Descreve atributos de bases candidatas a bpm nas missoes */
struct bases_m
{
    int b_id;             /* ID da base candidata */
    int dist;             /* distancia da base a missao */
    struct cjto_t *h_id;  /* conjunto de herois presentes na base */
    struct cjto_t *União; /* uniao das habilidades dos herois presentes na base */
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

/* Inicializa as estatisticas gerais do mundo */
/* Retorno: struct statistics */
struct statistics estatisticas_inicia(struct world *my_world);

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
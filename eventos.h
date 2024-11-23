
#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"

/* Cria um novo evento a ser inserio na LEF. */
/* Os campos inseridos no ev sao:
 * - Instante de tempo t (minutos) o qual o evento ocorrera;
 * - Tipo do evento inserido, variando de 1 a 10;
 * - Heroi vinculado ao evento, se for -1 nenhum heroi está vinculado;
 * - Base vinculada ao evento, se for -1 nenhuma base está vinculada;
 * - Missao vinculada ao evento, se for -1 nenhuma missao está vinculada; */
/* Retorna: ponteiro para uma struct event ou NULL em caso de erro. */
struct event *cria_evento(int t, int type, int h, int b, int m);

/* Evento o qual o heroi h chega em uma base b. */
/* O evento vincula o heroi h a nova base b. */
/* Verifica se o heroi espera ou nao na fila da base
 * - Se espera: cria e adiciona na LEF o evento ESPERA;
 * - Caso contrario: cria e adiciona na LEF o evento DESISTE; */
/* Imprime o instante t que o heroi h ESPERA/DESISTE na base b que tem x herois com capacidade para c herois */
void evento_chega(struct fprio_t **lef, struct world *my_world, struct event *ev);

/* Evento o qual o heroi h decide espera na fila de espera da base b. */
/* O evento adiciona o heroi a fila de espera da base b. */
/* Cria e adicona na LEF o evento AVISA */
/* Imprime o instante t que o heroi h espera na fila da base b com x herois na fila (sem contar ele) */
void evento_espera(struct fprio_t **lef, struct world *my_world, struct event *ev);

/* Evento o qual o heroi h desiste de tentar entrar na base b. */
/* O evento sorteia uma nova para o heroi h ir (podendo ser a mesma). */
/* Cria e adiciona na LEF o evento VIAJA. */
/* Imprime o instante t que o heroi h desiste de entrar na base b */
void evento_desiste(struct fprio_t **lef, struct world *my_world, struct event *ev);

/* Evento o qual o porteiro da base b é avisado e verifica quantos herois podem entrar na base. */
/* O evento retira os herois da fila da base b e inclui no conjunto de presentes da base quantos herois for possivel. */
/* Cria e adiciona na LEF o evento ENTRA para cada heroi retirado da fila. */
/* Imprime duas situacoes:
 * - Todos os herois que estao atualmente na fila de espera da base b
 * - Quais herois o porteiro da base admite entrar na base */
void evento_avisa(struct fprio_t **lef, struct world *my_world, struct event *ev);

/* Evento o qual o heroi h entra na base b. */
/* O evento calcula o tempo de permanencia do heroi na base b. */
/* Cria e adiciona na LEF o evento SAI. */
/* Imprime o instante t que o heroi h entra na base b com x herois presentes (incluindo ele) e com capacidade para c herois e quando ele saira da base. */
void evento_entra(struct fprio_t **lef, struct world *my_world, struct event *ev);

/* Evento o qual o heroi h sai da base b. */
/* O evento retira o heroi h do conjunto de heroi presentes da base b */
/* Cria e adiciona na LEF os eventos VIAJA e AVISA. */
/* Imprime o instante t que o heroi h sai da base b passando a ter x herois com capacidade para c herois. */
void evento_sai(struct fprio_t **lef, struct world *my_world, struct event *ev);

/* Evento o qual o heroi h viaja para a nova base d. */
/* O evento calcula a distancia entre as bases e o tempo que essa viagem durara. */
/* Cria e adiciona na LEF o evento CHEGA. */
/* Imprime o instante t que o heroi h vai da base b a base d, com distancia dist e velocidade de h. Ele chega novo instante t. */
void evento_viaja(struct fprio_t **lef, struct world *my_world, struct event *ev);

/* Evento o qual o heroi morre. */
/* O evento muda o status do heroi h para morto (status 0). */
/* Cria e adiciona na LEF o evento AVISA. */
/* Imprime o instante t que o heroi h morre na missao m. */
void evento_morre(struct fprio_t **lef, struct world *my_world, struct event *ev);

void evento_missao(struct fprio_t **lef, struct world *my_world, struct event *ev);

#endif
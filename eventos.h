
#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"

/* Sorteia um número aleatório entre min e max */
/* Retorna o número sorteado */
long aleat(long min, long max);

struct event *cria_item(int tipo, int prio, struct hero *h, int base);

void imprime_eventos(int tipo, int clock, struct event *ev, struct base *bases, int status);

/* Evento que ocorre quando um heroi chega na base */
/* Atualiza o campo base do heroi para a base do parametro */
/* Adiciona na LEF o evento ESPERA ou DESISTE */
/* Casos de retorno
 * -1: erro
 *  0: heroi desistiu
 *  1: heroi esperara na fila da base */
int chega(int t, struct fprio_t **lef, struct hero *h, struct base b);

/* Evento que ocorre quando um heroi decidi esperar na fila */
/* Atualiza o campo da fila da base b adicionando o heroi h nela */
/* Adiciona na LEF o evento AVISA */
/* Retorna 0 em caso de erro e 1 em caso de sucesso */
int espera(int t, struct fprio_t **lef, struct hero *h, struct base *b);

/* Evento que ocorre quando um heroi decidi nao entrar na fila */
/* É feito o sorteio de uma nova base para o heroi ir */
/* Adiciona na LEF o evento VIAJA */
/* Retorna 0 em caso de erro e 1 em caso de sucesso */
int desiste(int t, struct fprio_t **lef, struct hero h);

int avisa(int t, struct fprio_t **lef, struct hero *heroes, struct base *b);

int entra(int t, struct fprio_t **lef, struct hero *h, struct base *b, int *tbp);

int sai(int t, struct fprio_t **lef, struct hero *h, struct base *b);

int viaja(int t, struct fprio_t **lef, struct hero *h, struct base *bases,
          int current_base, int new_base, int *dist, int *duration);

int morre(int t, struct fprio_t **lef, struct hero *h, struct base *b);

#endif
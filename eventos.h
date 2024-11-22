
#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"

/* Sorteia um número aleatório entre min e max */

struct event *cria_evento(int t, int type, int h, int b);

void evento_chega(struct fprio_t **lef, struct world *my_world, struct event *ev);

void evento_espera(struct fprio_t **lef, struct world *my_world, struct event *ev);

void evento_desiste(struct fprio_t **lef, struct world *my_world, struct event *ev);

void evento_avisa(struct fprio_t **lef, struct world *my_world, struct event *ev);

#endif
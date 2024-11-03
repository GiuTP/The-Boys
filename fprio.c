// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria ()
{
    struct fprio_t *f;
    
    if (!(f = malloc(sizeof(struct fprio_t))))
        return NULL;

    f->prim = NULL;
    f->num = 0;

    return f;
}

struct fprio_t *fprio_destroi (struct fprio_t *f)
{
    struct fpnodo_t *aux;

    if (f == NULL)
        return NULL;

    while (f->prim != NULL)
    {
        aux = f->prim;
        f->prim = aux->prox; 

        free(aux); /* libera o nodo */
    }

    free(f); /* libera a fila */

    return NULL;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
    struct fpnodo_t *aux, *novo;

    if (f == NULL || item == NULL)
        return -1;


    if (!(novo = malloc(sizeof(struct fpnodo_t))))
        return -1;

    novo->item = item;
    novo->tipo = tipo;
    novo->prio = prio;

    if (f->num == 0)
    {
        f->prim = novo;
        novo->prox = NULL;
        f->num++;

        return f->num;
    }

    aux = f->prim;

    if (prio <= aux->prio)
    {
        novo->prox = aux;
        f->prim = novo;
        if (aux->item == item && aux->prio == prio && aux->tipo == tipo)
            return -1;

        f->num++;
        return f->num;
    }

    while (aux->prox != NULL && prio >= aux->prox->prio)
    {
        if (aux->item == item && aux->prio == prio && aux->tipo == tipo)
            return -1;
        aux = aux->prox;
    }

    novo->prox = aux->prox;
    aux->prox = novo;
    f->num++;


    return f->num;    
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
    struct fpnodo_t *aux;
    void *item;
    if (f == NULL || f->prim == NULL || tipo == NULL || prio == NULL)
        return NULL;

    aux = f->prim;
    
    if (f->num == 1)
        f->prim = NULL;
    else
        f->prim = aux->prox;

    *tipo = aux->tipo;
    *prio = aux->prio;
    item = aux->item;

    free(aux);
    f->num--;

    return item;
}

int fprio_tamanho (struct fprio_t *f)
{
    if (f == NULL)
        return -1;
    
    return f->num;
}

void fprio_imprime (struct fprio_t *f)
{
    struct fpnodo_t *aux;
    int i;

    if (f == NULL || f->num == 0)
        return;

    aux = f->prim;
    for (i = 0; i < f->num; i++)
    {
        printf("(%d %d)", aux->tipo, aux->prio);
        if (i < f->num - 1)
        printf(" ");

        aux = aux->prox;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria ()
{
    struct fprio_t *f;
    
    if (!(f = malloc(sizeof(struct fprio_t))))
        return NULL;

    /* Lista não aponta para nenhum nodo na sua criação e possui tamanho 0 */
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
    
        free(aux->item); /* libera o item do nodo */
        free(aux); /* libera o nodo */
    }

    free(f); /* libera a fila */

    return NULL;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
    struct fpnodo_t *aux, *novo;

    /* Verifica se fila ou item são ponteiros nulos */
    if (f == NULL || item == NULL)
        return -1;

    if (!(novo = malloc(sizeof(struct fpnodo_t))))
        return -1;

    /* Novo nodo recebe os parâmetros a serem incluídos */
    novo->item = item;
    novo->tipo = tipo;
    novo->prio = prio;

    /* Verifica se o item já existe na fila */
    aux = f->prim;
    while (aux != NULL)
    {
        if ((aux->item == item) && (aux->tipo == tipo) && (aux->prio == prio))
        {
            free(novo);
            return -1;
        }

        aux = aux->prox;
    }

    /* Insere em fila vazia */
    if (f->num == 0)
    {
        f->prim = novo;
        novo->prox = NULL;
        f->num++;

        return f->num;
    }

    aux = f->prim;

    /* Caso onde a prioridade do novo nodo é maior que o primeiro nodo da fila */
    if (prio < aux->prio)
    {
        novo->prox = aux;
        f->prim = novo;
        f->num++;

        return f->num;
    }

    /* Caso geral, tenta inserir pelo meio */
    while (aux->prox != NULL && prio >= aux->prox->prio)
        aux = aux->prox;

    novo->prox = aux->prox;
    aux->prox = novo;
    f->num++;

    return f->num;    
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
    struct fpnodo_t *aux;
    void *item;

    /* Verifica se nenhum ponteiro é nulo */
    if (f == NULL || f->prim == NULL || tipo == NULL || prio == NULL)
        return NULL;

    /* Retira conforme política FIFO */
    aux = f->prim;

    f->prim = aux->prox;
    *tipo = aux->tipo; /* devolve o tipo do item retirado */
    *prio = aux->prio; /* devolve a prioridade do item retirado */
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

    /* Se fila for vazia ou nula, não imprime nada */
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
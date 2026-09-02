#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila_t *fila_cria()
{
    struct fila_t *queue;

    if (!(queue = malloc(sizeof(struct fila_t))))
        return NULL;

    /* fila não aponta para nenhum nodo na sua criação e possui tamanho 0 */
    queue->prim = NULL;
    queue->ult = NULL;
    queue->tamanho = 0;

    return queue;
}

struct fila_t *fila_destroi(struct fila_t *fl)
{
    struct item_t *aux;

    while (fl->prim != NULL)
    {
        aux = fl->prim;
        fl->prim = aux->prox;
        free(aux);
    }

    free(fl);

    return NULL;
}

int enqueue(struct fila_t *fl, int item, int pos)
{
    struct item_t *novo;

    /* Caso de fila nula */
    if (fl == NULL)
        return -1;

    if (!(novo = malloc(sizeof(struct item_t))))
        return -1;

    /* Novo nodo recebe o item a ser incluído na fila */
    novo->valor = item;

    /* Inserção em fila vazia */
    if (fl->tamanho == 0)
    {
        fl->prim = novo;
        fl->ult = novo;
        novo->ant = NULL;
        novo->prox = NULL;
    }

    /* Inserção na fila (última posição) */
    if (pos == -1)
    {
        fl->ult->prox = novo;
        novo->ant = fl->ult;
        novo->prox = NULL;
        fl->ult = novo;
    }

    fl->tamanho++; /* Incrementa o tamanho da fila */

    return fl->tamanho;
}

int dequeue(struct fila_t *fl, int *item, int pos)
{
    struct item_t *aux;

    /* Caso de fila nula */
    if (fl == NULL)
        return -1;

    /* Politica FIFO */
    if (pos >= 0)
    {
        aux = fl->prim;     /* Aux recebe o primeiro nodo da fila */
        *item = aux->valor; /* Item recebe o valor do primeiro nodo */

        fl->prim = aux->prox; /* Ponteiro de primeiro aponta para o segundo da fila */
        if (fl->prim != NULL)
            fl->prim->ant = NULL; /* Ponteiro de anterior do segundo (novo 1) aponta para NULL */
        else
            fl->ult = NULL; /* Ponteiro de ultimo aponta para NULL */
    }

    free(aux);
    fl->tamanho--;

    return fl->tamanho;
}

int lista_consulta(struct fila_t *fl, int *item, int pos)
{
    struct item_t *aux;
    int i;

    /* Casos de fila nula ou posição inválida para consulta */
    if (fl == NULL || pos < -1 || pos >= fl->tamanho)
        return -1;

    /* Consulta na última posição da fila */
    if (pos == -1)
    {
        *item = fl->ult->valor;

        return fl->tamanho;
    }

    /* Consulta em uma posição qualquer da fila */
    aux = fl->prim;
    for (i = 0; i < pos; i++)
        aux = aux->prox;

    *item = aux->valor;

    return fl->tamanho;
}

int lista_procura(struct fila_t *fl, int valor)
{
    struct item_t *aux;
    int pos;

    /* Caso de fila nula */
    if (fl == NULL)
        return -1;

    /* Loop acaba por dois motivos:
     * Ou o valor desejado foi encontrado;
     * Ou o pos chegou até o fim e não achou o valor. */
    aux = fl->prim;
    for (pos = 0; pos < fl->tamanho && aux->valor != valor; pos++)
        aux = aux->prox;

    if (pos == fl->tamanho)
        return -1;

    return pos;
}

int fila_tamanho(struct fila_t *fl)
{
    if (fl == NULL)
        return -1;

    return fl->tamanho;
}

void fila_imprime(struct fila_t *fl)
{
    struct item_t *aux;
    int i;

    /* Se a fila for nula ou de tamanho 0 não imprime nenhum valor */
    if (fl == NULL || fl->tamanho == 0)
        return;

    aux = fl->prim;
    for (i = 0; i < fl->tamanho; i++)
    {
        printf("%d", aux->valor);
        if (i < fl->tamanho - 1)
            printf(" ");

        aux = aux->prox;
    }

    return;
}
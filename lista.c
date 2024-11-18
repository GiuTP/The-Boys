// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct lista_t *lista_cria()
{
    struct lista_t *lst;

    if (!(lst = malloc(sizeof(struct lista_t))))
        return NULL;

    /* Lista não aponta para nenhum nodo na sua criação e possui tamanho 0 */
    lst->prim = NULL;
    lst->ult = NULL;
    lst->tamanho = 0;

    return lst;
}

struct lista_t *lista_destroi(struct lista_t *lst)
{
    struct item_t *aux;

    while (lst->prim != NULL)
    {
        aux = lst->prim;
        lst->prim = aux->prox;
        free(aux);
    }

    free(lst);

    return NULL;
}

/* Adaptado para fila (FIFO) */
int lista_insere(struct lista_t *lst, int item, int pos)
{
    struct item_t *novo;

    /* Caso de lista nula */
    if (lst == NULL)
        return -1;

    /* Aloca memória para novo nodo */
    if (!(novo = malloc(sizeof(struct item_t))))
        return -1;

    /* Novo nodo recebe o item a ser incluído na fila */
    novo->valor = item;

    /* Inserção em lista vazia */
    if (lst->tamanho == 0)
    {
        lst->prim = novo;
        lst->ult = novo;
        novo->ant = NULL;
        novo->prox = NULL;
    }

    /* Inserção na fila (última posição) */
    if (pos == -1)
    {
        lst->ult->prox = novo;
        novo->ant = lst->ult;
        novo->prox = NULL;
        lst->ult = novo;
    }

    lst->tamanho++; /* Incrementa o tamanho da lista */

    return lst->tamanho;
}

/* Adaptado para fila (FIFO) */
int lista_retira(struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux;

    /* Caso de lista nula */
    if (lst == NULL)
        return -1;

    /* Politica FIFO */
    if (pos >= 0)
    {
        aux = lst->prim;    /* Aux recebe o primeiro nodo da fila */
        *item = aux->valor; /* Item recebe o valor do primeiro nodo */

        lst->prim = aux->prox; /* Ponteiro de primeiro aponta para o segundo da fila */
        if (lst->prim != NULL)
            lst->prim->ant = NULL; /* Ponteiro de anterior do segundo (novo 1) aponta para NULL */
        else
            lst->ult = NULL; /* Ponteiro de ultimo aponta para NULL */
    }

    free(aux);
    lst->tamanho--;

    return lst->tamanho;
}

int lista_consulta(struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux;
    int i;

    /* Casos de fila nula ou posição inválida para consulta */
    if (lst == NULL || pos < -1 || pos >= lst->tamanho)
        return -1;

    /* Consulta na última posição da fila */
    if (pos == -1)
    {
        *item = lst->ult->valor;

        return lst->tamanho;
    }

    /* Consulta em uma posição qualquer da fila */
    aux = lst->prim;
    for (i = 0; i < pos; i++)
        aux = aux->prox;

    *item = aux->valor;

    return lst->tamanho;
}

int lista_procura(struct lista_t *lst, int valor)
{
    struct item_t *aux;
    int pos;

    /* Caso de fila nula */
    if (lst == NULL)
        return -1;

    /* Loop acaba por dois motivos:
     * Ou o valor desejado foi encontrado;
     * Ou o pos chegou até o fim e não achou o valor. */
    aux = lst->prim;
    for (pos = 0; pos < lst->tamanho && aux->valor != valor; pos++)
        aux = aux->prox;

    if (pos == lst->tamanho)
        return -1;

    return pos;
}

int lista_tamanho(struct lista_t *lst)
{
    if (lst == NULL)
        return -1;

    return lst->tamanho;
}

void lista_imprime(struct lista_t *lst)
{
    struct item_t *aux;
    int i;

    /* Se a fila for nula ou de tamanho 0 não imprime nenhum valor */
    if (lst == NULL || lst->tamanho == 0)
        return;

    aux = lst->prim;
    for (i = 0; i < lst->tamanho; i++)
    {
        printf("%d", aux->valor);
        if (i < lst->tamanho - 1)
            printf(" ");

        aux = aux->prox;
    }
}
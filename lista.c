// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct lista_t *lista_cria ()
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

struct lista_t *lista_destroi (struct lista_t *lst)
{
    struct item_t *aux;

    while (lst->prim != NULL)
    {
        aux = lst->prim;
        lst->prim = aux->prox;
        free(aux);
    }

    free (lst);

    return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos)
{
    struct item_t *aux, *novo;
    int i;

    /* Verifica se lista nula ou posição inválida para inserção */
    if (lst == NULL || pos < -1)
        return -1;
    
    if (!(novo = malloc(sizeof(struct item_t))))
        return -1;
    
    /* Novo nodo recebe o item a ser incluído na posição pos */
    novo->valor = item;

    /* Inserção em lista vazia (tamanho 0) */
    if (lst->tamanho == 0)
    {
        lst->prim = novo;
        lst->ult = novo;
        novo->ant = NULL;
        novo->prox = NULL;

        lst->tamanho++;

        return lst->tamanho;
    }

    /* Inserção na última posição da lista */
    if (pos == -1)
    {
        lst->ult->prox = novo;
        novo->ant = lst->ult;
        novo->prox = NULL;
        lst->ult = novo;

        lst->tamanho++;

        return lst->tamanho;
    }

    /* Inserção na primeira posição da lista */
    if (pos == 0)
    {
        lst->prim->ant = novo;
        novo->ant = NULL;
        novo->prox = lst->prim;
        lst->prim = novo;

        lst->tamanho++;

        return lst->tamanho;
    }

    /* Inserção no meio da lista na posição pos */
    aux = lst->prim;
    for (i = 0; i < pos - 1; i++)
        aux = aux->prox;
    
    novo->prox = aux->prox;
    novo->ant = aux;
    aux->prox->ant = novo;
    aux->prox = novo;

    lst->tamanho++;

    return lst->tamanho;
}

int lista_retira (struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux;
    int i;

    /* Casos de lista nula ou posição inválida de retirada */
    if (lst == NULL || pos < -1 || pos >= lst->tamanho)
        return -1;

    /* Remoção de lista de tamanho 1 */
    if (lst->tamanho == 1)
    {
        aux = lst->prim;
        *item = aux->valor;

        lst->prim = NULL;
        lst->ult = NULL;

        free(aux);
        lst->tamanho--;

        return lst->tamanho;
    }

    /* Remoção da última posição da lista */
    if (pos == -1)
    {
        aux = lst->ult;
        *item = aux->valor;

        lst->ult = aux->ant;
        lst->ult->prox = NULL;

        free(aux);
        lst->tamanho--;

        return lst->tamanho;
    }

    /* Remoção da primeira posição da lista */
    if (pos == 0)
    {
        aux = lst->prim;
        *item = aux->valor;

        lst->prim = aux->prox;
        lst->prim->ant = NULL;

        free(aux);
        lst->tamanho--;

        return lst->tamanho;
    }

    /* Remoção no meio da lista na posição pos */
    aux = lst->prim;
    for(i = 0; i < pos; i++)
        aux = aux->prox;
    
    *item = aux->valor;
    aux->ant->prox = aux->prox;
    aux->prox->ant = aux->ant;

    free(aux);
    lst->tamanho--;
    
    return lst->tamanho;
}

int lista_consulta (struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux;
    int i;

    /* Casos de lista nula ou posição inválida para consulta */
    if (lst == NULL || pos < -1 || pos >= lst->tamanho)
        return -1;
    
    /* Consulta na última posição da lista */
    if (pos == -1)
    {
        *item = lst->ult->valor;

        return lst->tamanho;
    }

    /* Consulta em uma posição qualquer da lista */
    aux = lst->prim;
    for (i = 0; i < pos; i++)
        aux = aux->prox;
    
    *item = aux->valor;

    return lst->tamanho;
}

int lista_procura (struct lista_t *lst, int valor)
{
    struct item_t *aux;
    int pos;

    /* Caso de lista nula */
    if (lst == NULL)
        return -1;

    /* Loop acaba por dois motivos:
        * Ou o valor desejado foi encontrado;
        * Ou o pos chegou até o fim e não achou o valor. */
    aux = lst->prim;
    for(pos = 0; pos < lst->tamanho && aux->valor != valor; pos++)
        aux = aux->prox;
    
    if (pos == lst->tamanho)
        return -1;

    return pos;
}

int lista_tamanho (struct lista_t *lst)
{
    if (lst == NULL)
        return -1;

    return lst->tamanho;
}

void lista_imprime (struct lista_t *lst)
{
    struct item_t *aux;
    int i;

    /* Se a lista for nula ou de tamanho 0 não imprime nenhum valor */
    if (lst == NULL || lst->tamanho == 0)
        return;

    aux = lst->prim;
    for (i = 0; i < lst->tamanho; i++)
    {
        printf("%d", aux->valor);
        if(i < lst->tamanho - 1)
            printf(" ");

        aux = aux->prox;
    }
}
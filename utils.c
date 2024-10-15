#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

void ler_v (struct racional **v, int n)
{
    int i;
    long num, den;

    for (i = 0; i < n; i++)
    {
        scanf("%ld %ld", &num, &den);
        v[i] = cria_r(num, den);
    }
}

void imprime_v (struct racional **v, int n)
{
    int i;

    printf("VETOR = ");
    for (i = 0; i < n; i++)
    {
        imprime_r(v[i]);
        printf(" ");
    }

    printf("\n");
}

void elimina_NaN (struct racional **v, int *n)
{
    int i;

    for (i = 0; i < *n; i++)
    {
        if (!valido_r(v[i]))
        {
            destroi_r(v[i]);
            v[i] = v[*n - 1];
            (*n)--; /* Decrementa o tamanho de v a cada NaN removido */
            i--;
        }
    }
}

void ordenar_v (struct racional **v, int n)
{
    int i, j;
    struct racional *temp;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - i -1; j++)
        {
            if (compara_r(v[j], v[j+1]) == 1)
            {
                temp = v[j];
                v[j] = v[j+1];
                v[j+1] = temp;
            }
        }
    }
}

int soma_v (struct racional **v, int n, struct racional **sum)
{
    int i;

    if (!(*sum = malloc(sizeof(struct racional))))
        return 0;
    
    /* Inicializa o acumulador */
    (*sum)->num = 0;
    (*sum)->den = 1;

    printf("SOMA = ");
    for (i = 0; i < n; i++)
    {
        if (!valido_r(v[i]))
            return 0;
        

        soma_r(*sum, v[i], *sum);
    }
    
    return 1;
}

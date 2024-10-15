#include <stdio.h>
#include <stdlib.h>
#include "racional.h"
#include "utils.h"

int main ()
{
    struct racional **v;
    struct racional *sum;
    int n;

    scanf ("%d", &n);
    if (n <= 0 || n >= 100)
        return -1;

    if ( !(v = malloc(sizeof(struct racional*) * n)) )
    {
        fprintf(stderr, "Alocacao de memoria falhou :(\n)");
        return -1;
    }

    ler_v(v, n);
    imprime_v(v, n);

    elimina_NaN(v, &n);
    imprime_v(v, n);

    ordenar_v(v, n);
    imprime_v(v, n);

    soma_v(v, n, &sum);
    imprime_r(sum);
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        destroi_r(v[i]);
        v[i] = NULL;
    }
    
    imprime_v(v,n);
    free(v);
    free(sum);
    
    return 0;
}


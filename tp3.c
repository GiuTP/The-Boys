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

    /* Checagem para verificar se a memória 
       do vetor foi alocadada com sucesso   */
    if ( !(v = malloc(sizeof(struct racional) * n)) )
        return -1;

    /* Ler um vetor de n ponteiros e o imprime  */
    ler_v(v, n);
    imprime_v(v, n);

    /* Elimina os NaN de um vetor e o imprime      */
    elimina_NaN(v, &n);
    imprime_v(v, n);

    /* Orderna um vetor e o imprime              */
    ordenar_v(v, n);
    imprime_v(v, n);

    /* Soma os elementos e imprime o resultado  */
    soma_v(v, n, &sum);
    imprime_r(sum);
    printf("\n");
    
    /* Libera os racionais do vetor e o imprime */
    for (int i = 0; i < n; i++)
    {
        destroi_r(v[i]);
        v[i] = NULL;
    }    
    imprime_v(v,n);

    /* Libera a memória dos n ponteiros do vetor
     * Libera a memória da soma dos elementos   */
    free(v);
    free(sum);
    
    return 0;
}


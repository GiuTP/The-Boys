/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"


/* programa principal */
int main ()
{
    srand (0);

    int i; /* contador do laço */
    long n; /* limite de interações */
    long max; /* máximo (e mínimo) dos números sorteados */
    struct racional r1, r2; /* primeiros racionais r */
    struct racional rSum, rSub, rMult, rDiv; /* operações feitas em r1 e r2 */

    /* checa se o n e max lidos estão no intervalo certo*/
    do
        scanf("%ld %ld", &n, &max);
    while ((n <= 0) || (n >= 100) || (max <= 0) || (max >= 30));


    for(i = 1; i <= n; i++)
    {
        printf("%d: ", i); /* imprime i de 1 até n */

        /* criação dos elementos r1 e r2 */
        r1 = sorteia_r(-max, max);
        r2 = sorteia_r(-max, max);

        /* imprime r1 e r2 */
        imprime_r(r1);
        printf(" ");
        imprime_r(r2);
        printf(" ");
        
        /* se algum r for inválido, o programa termina aqui */
        if (!(valido_r(r1)) || !(valido_r(r2)))
        {
            printf("NUMERO INVALIDO\n");
            return 1;
        }
        
        /* se a divisão for inválida, o programa termina aqui */
        if (!(r2.num))
        {
            printf("DIVISAO INVALIDA\n");
            return 1;
        }

        /* operações realizadas em r1 e r2 */
        rSum = soma_r(r1, r2);
        rSub = subtrai_r(r1, r2);
        rMult = multiplica_r(r1, r2);
        rDiv = divide_r(r1, r2);

        /* imprime as operações feitas em r1 e r2 */
        imprime_r(rSum);
        printf(" ");
        imprime_r(rSub);
        printf(" ");
        imprime_r(rMult);
        printf(" ");
        imprime_r(rDiv);

        /* troca de linha após acabar a iteração atual */
        printf("\n");
    }

    return 0;
}
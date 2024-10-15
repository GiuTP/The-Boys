/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Maximo Divisor Comum entre a e b            */
/* Utilizado o algoritmo de Euclides recursivo */
long mdc (long a, long b)
{
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    
    if (b == 0)
        return a;
    
    return mdc (b, a%b);
}

/* Minimo Multiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b)
{
  return (a * b) / mdc(a, b);
}

/* Simplifica o número racional indicado no parâmetro.
 * Por exemplo, se o número for 10/8 muda para 5/4.
 * Retorna 1 em sucesso e 0 se r for inválido ou o ponteiro for nulo.
 * Se ambos numerador e denominador forem negativos, o resultado é positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador. */
int simplifica_r (struct racional *r)
{
    long div_com;

    if (!valido_r(r))
        return 0;

    if (r->den < 0)
    {
        r->num = -r->num;
        r->den = -r->den;
    }
    
    div_com = mdc(r->num, r->den);
    r->num /= div_com;
    r->den /= div_com;

    return 1;
}

struct racional *cria_r (long numerador, long denominador)
{
    struct racional *r;

    if ( !( r = malloc (sizeof (struct racional) ) ) )
        return NULL;

    r->num = numerador;
    r->den = denominador;

    return r;
}

void destroi_r (struct racional *r)
{
    free(r);
}

int valido_r (struct racional *r)
{
    if (r == NULL || r->den == 0)
        return 0;

    return 1;
}

void imprime_r (struct racional *r)
{
    if (r == NULL)
    {
        printf("NULL");
        return;
    }

    simplifica_r(r);
    if (!valido_r(r))
    {
        printf("NaN");
        return;
    }
    if (r->num == 0 || r->den == 1 || r->num == r->den)
    {
        printf("%ld", r->num);
        return;
    }

    printf("%ld/%ld", r->num, r->den);
}

int compara_r (struct racional *r1, struct racional *r2)
{
    long num1, num2;

    if (!valido_r (r1) || !valido_r (r2))
        return -2;
    
    num1 = mmc(r1->den, r2->den) / r1->den * r1->num;
    num2 = mmc(r1->den, r2->den) / r2->den * r2->num;

    if (num1 < num2)
        return -1;
    
    if (num1 == num2)
        return 0;
    
    return 1;
}

int soma_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    long num1, num2;
    long div_com;
    
    if (!valido_r (r1) || !valido_r (r2) || r3 == NULL)
        return 0;
    
    div_com = mmc(r1->den,r2->den);
    num1 = (div_com / r1->den) * r1->num;
    num2 = (div_com / r2->den) * r2->num;

    r3->num = num1 + num2;
    r3->den = div_com;

    return 1;
}

int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    long num1, num2;
    long div_com;

    if (!valido_r (r1) || !valido_r (r2) || r3 == NULL)
        return 0;
    
    div_com = mmc(r1->den,r2->den);
    num1 = (div_com / r1->den) * r1->num;
    num2 = (div_com / r2->den) * r2->num;

    r3->num = num1 - num2;
    r3->den = div_com;

    return 1;
}

int multiplica_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!valido_r (r1) || !valido_r (r2) || r3 == NULL)
        return 0;
    
    r3->num = r1->num * r2->num;
    r3->den = r1->den * r2->den;
    simplifica_r(r3);

    return 1;
}

int divide_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!valido_r (r1) || !valido_r (r2) || r2->num != 0 || r3 == NULL)
        return 0;

    r3->num = r1->num * r2->den;
    r3->den = r1->den * r2->num;
    simplifica_r(r3);

    return 1;
}
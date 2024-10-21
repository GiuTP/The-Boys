#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Maximo Divisor Comum entre a e b            */
/* Utilizado o algoritmo de Euclides recursivo */
long mdc (long a, long b)
{
    if (a < 0) /* Módulo de a */
        a = -a; 
    if (b < 0) /* Módulo de b */
        b = -b;
    
    if (b == 0)
        return a;
    
    return mdc(b, a%b);
}

/* Minimo Multiplo Comum entre a e b */
long mmc (long a, long b)
{
  return (a * b) / mdc(a, b);
}

/* Recebe um número racional e o simplifica
    * Retorna 1 caso o r for simplificado com sucesso ou
    *         0 caso o r for um valor considerado inválido */
int simplifica_r (struct racional *r)
{
    long div_com;

    if (!valido_r(r))
        return 0;

    /* Casos onde temos:
        * Denominador negativo o sinal é transferido para o numerador
        * Ambos negativos o racional é simplificado para positivo     */
    if (r->den < 0)
    {
        r->num = -r->num;
        r->den = -r->den;
    }

    /* Se mdc do r não for 1, é simplificado para a fração irredutível */
    div_com = mdc(r->num, r->den);
    r->num /= div_com;
    r->den /= div_com;

    return 1;
}

struct racional *cria_r (long numerador, long denominador)
{
    struct racional *r;

    if (!(r = malloc(sizeof(struct racional))))
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
    
    /* É comparado apenas os numeradores dos r */
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
    
    /* Deixa os r no mesmo denominador se necessário */
    div_com = mmc(r1->den,r2->den);
    num1 = (div_com / r1->den) * r1->num;
    num2 = (div_com / r2->den) * r2->num;

    r3->num = num1 + num2;
    r3->den = div_com;
    simplifica_r(r3);

    return 1;
}

int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    long num1, num2;
    long div_com;

    if (!valido_r (r1) || !valido_r (r2) || r3 == NULL)
        return 0;
    
    /* Deixa os r no mesmo denominador se necessário */
    div_com = mmc(r1->den,r2->den);
    num1 = (div_com / r1->den) * r1->num;
    num2 = (div_com / r2->den) * r2->num;

    r3->num = num1 - num2;
    r3->den = div_com;
    simplifica_r(r3);

    return 1;
}

int multiplica_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!valido_r (r1) || !valido_r (r2) || r3 == NULL)
        return 0;
    
    /* Utilizado método matemático de multiplicação de frações */
    r3->num = r1->num * r2->num;
    r3->den = r1->den * r2->den;
    simplifica_r(r3);

    return 1;
}

int divide_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    if (!valido_r (r1) || !valido_r (r2) || r2->num != 0 || r3 == NULL)
        return 0;

    /* Utilizado método matemático de divisão de frações */
    r3->num = r1->num * r2->den;
    r3->den = r1->den * r2->num;
    simplifica_r(r3);

    return 1;
}
/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max)
{
  return rand() % (max - min + 1) + min;
}

/* Máximo Divisor Comum entre a e b      */
/* calcula o MDC pelo método de Euclides */
long mdc (long a, long b)
{
  if (a < 0) /* módulo de a */
    a = -a;
  if (b < 0) /* módulo de b */
    b = -b;
  
  if (!(b))
    return a;
  
  return mdc(b, a%b);
}

/* Mínimo Múltiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b)
{
  return a*b/mdc(a,b);
}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r (struct racional r)
{
  long div_com;

  if (!valido_r(r))
    return r;

  if (r.den < 0)
  {
    r.num = -r.num;
    r.den = -r.den;
  }

  if (mdc(r.num, r.den) != 1)
  {
    div_com = mdc(r.num, r.den);
    r.num /= div_com;
    r.den /= div_com;
  }

  return r;
}

struct racional cria_r (long numerador, long denominador)
{
  struct racional r;

  r.num = numerador;
  r.den = denominador;

  return r;
}

int valido_r (struct racional r)
{
  if (!r.den)
    return 0;

  return 1;
}

struct racional sorteia_r (long min, long max)
{
  struct racional r;
  long numerador, denominador;
  
  numerador = aleat(min, max);
  denominador = aleat(min, max);
  r = cria_r(numerador, denominador);

  return simplifica_r(r);
}

void imprime_r (struct racional r)
{
  r = simplifica_r(r);

  if (!valido_r(r))
  {
    printf("NaN");
    return;
  }

  if (!(r.num) || (r.den == 1) || (r.num == r.den))
  {
    printf("%ld", r.num);
    return;
  }

  printf("%ld/%ld", r.num, r.den);
}

int compara_r (struct racional r1, struct racional r2)
{
  long num1, num2;
  long div_com;

  if (!valido_r(r1) || !valido_r(r2))
    return -2;

  r1 = simplifica_r(r1);
  r2 = simplifica_r(r2);

  num1= r1.num;
  num2= r2.num;
  if (r1.den != r2.den)
  {
    div_com = mmc(r1.den, r2.den);
    num1 = div_com / r1.den * r1.num;
    num2 = div_com / r2.den * r2.num;
  }

  if (num1 < num2)
    return -1;
  
  if (num1 == num2)
    return 0;

  return 1;
}

int soma_r (struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || r3 == NULL)
    return 0;

  if (r1.den != r2.den)
  {
    r3->den = mmc(r1.den, r2.den);
    r3->num = r3->den/r1.den*r1.num + r3->den/r2.den*r2.num;
    return 1;
  }

  r3->num = r1.num + r2.num;
  r3->den = r1.den;

  return 1;
}

int subtrai_r (struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || r3 == NULL)
    return 0;

  if (r1.den != r2.den)
  {
    r3->den = mmc(r1.den, r2.den);
    r3->num = r3->den/r1.den*r1.num - r3->den/r2.den*r2.num;
    return 1;
  }

  r3->num = r1.num - r2.num;
  r3->den = r1.den;

  return 1;
}

int multiplica_r (struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || r3 == NULL)
    return 0;
  
  r3->num = r1.num*r2.num;
  r3->den = r1.den*r2.den;

  return 1;
}

int divide_r (struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || !r2.num ||r3 == NULL)
    return 0;

  r3->num = r1.num*r2.den;
  r3->den = r1.den*r2.num;

  return 1;
}
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

void ler_v (struct racional *v, int i)
{
  long num, den;

  scanf("%ld %ld", &num, &den);
  
  v[i].num = num;
  v[i].den = den;
}

int valido_r (struct racional r)
{
  if (!r.den)
    return 0;

  return 1;
}

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


int compara_r (struct racional r1, struct racional r2)
{
  long div_com;

  if (!valido_r(r1) || !valido_r(r2))
    return -2;

  if (r1.den != r2.den)
  {
    div_com = mmc(r1.den, r2.den);
    r1.num = div_com/r1.den * r1.num;
    r2.num = div_com/r2.den * r2.num;
  }

  if (r1.num < r2.num)
    return -1;
  
  if (r1.num > r2.num)
    return 1;

  return 0;
}

int main(){
    struct racional r1 = {6, 11};
    struct racional r2 = {-5, 1};
    struct racional v[100];
    ler_v(v, 15);

    for (int i = 0; i < 15; i++)
    {
        for (int j = i+1; j < 15; j++)
        {
            int test = compara_r(v[i], v[j]);
            printf ("v[%d](%ld/%ld) é maior/menor que v[%d](%ld/%ld)\n", i, v[i].num, v[i]. den, j, v[j].num, v[j].den);
        }
    }


    return 0;
}
/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max)
{
  return rand() % (max - min + 1) + min;
}

/* Máximo Divisor Comum entre a e b      */
/* Calcula o MDC pelo método de Euclides */
/* Tira o módulo de número se necessário */
long mdc (long a, long b)
{
  if (a < 0) /* módulo de a */ 
    a = -a;  
  if (b < 0) /* módulo de b */ 
    b = -b;  

  /* caso base, se b for 0, a é o mdc */
  if (!(b))
    return a;
  return mdc(b, a%b); 
}

/* Mínimo Múltiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b)
{
  if (a < 0) /* módulo de a */ 
    a = -a;  
  if (b < 0) /* módulo de b */ 
    b = -b; 
  
  return (a * b)/mdc(a, b);
}

/* Recebe um número racional e o simplifica seguindo as regras:
  * Se um o mdc deles for diferente de 1, é tirado o mdc e divido eles (10/8 vira 5/4);
  * Se ambos forem negativos a fração e convertida para positivo;
  * Se o denominador é negativo o sinal é transferido para o número;
  * Se r for inválido é feito retorno sem simplificação;                                */
struct racional simplifica_r (struct racional r)
{
  if (!valido_r(r))
    return r;

  /* simplifica os casos onde: 
    * apenas o den é negativo e muda o sinal para o num
    * os dois valores são negativos e assim o r fica positivo */
  if (r.den < 0)
  {
    r.num = -r.num;
    r.den = -r.den;
  }

  /* verica se o mdc do racional é diferente de 1 */
  long div_com = mdc(r.num, r.den);
  if (div_com != 1 )
  {
    r.num /= div_com;
    r.den /= div_com;
  }

  return r;
}

/* Cria um número racional com o numerador e denominador indicados. */
struct racional cria_r (long numerador, long denominador)
{
  struct racional r;
  r.num = numerador;
  r.den = denominador;
  return r;
}

/* Verifica se o racional informado é válido
 * Um racional inválido é um racional com denominador 0
 * Retorna 1 se o racional r for válido ou 0 se for inválido. */
int valido_r (struct racional r)
{
  if (!(r.den))
    return 0;
  return 1;
}

/* Retorna um número racional aleatório na forma simplificada.
 * Deve ser sorteado o numerador e depois o denominador.
 * o racional gerado pode ser válido ou inválido.
 * O numerador e o denominador devem ser inteiros entre min e max. */
struct racional sorteia_r (long min, long max)
{
  struct racional r;
  long numerador, denominador;

  numerador = aleat(min, max);
  denominador = aleat(min, max);
  r = cria_r(numerador, denominador);

  return simplifica_r(r);
}

/* Imprime um racional r, respeitando estas regras:
  * Não será imprimido espaços em branco ou quebra de linhas;
  * Se o r for inválido imprime "INVALIDO";
  * Se o numerador for 0 imprime apenas "0";
  * Se o denominador for 1, imprime apenas o numerador;
  * Se o r for negativo o sinal fica na frente do num (pela simplifica_r);
  * Se ambos forem negativos (num e den) o r é positiva;                    */
void imprime_r (struct racional r)
{
  r = simplifica_r(r);

  if (!(valido_r(r)))
  {
    printf("INVALIDO");
    return;
  }
  if (!(r.num) || (r.den == 1) || (r.num == r.den))
  {
    printf("%ld", r.num);
    return;
  }

  printf("%ld/%ld", r.num, r.den);
}

/* Retorna a soma dos racionais r1 e r2.
 * se r1 ou r2 for inválido, o resultado deve ser inválido */
struct racional soma_r (struct racional r1, struct racional r2)
{
  if (!(valido_r(r1)))
    return r1;
  if (!(valido_r(r2)))
    return r2;
  
  long num, den;
  /* denominadores diferentes calcula o mmc primeiro */
  if (r1.den != r2.den)
  {
    den = mmc(r1.den, r2.den);
    num = ((den/r1.den)*r1.num) + 
          ((den/r2.den)*r2.num);
    return cria_r(num, den);
  }

  /* denominadores iguais apenas soma os numeradores */
  num = r1.num + r2.num;
  den = r1.den;
  return cria_r(num, den);
}

/* Retorna a subtração dos racionais r1 e r2.
 * se r1 ou r2 for inválido, o resultado deve ser inválido */
struct racional subtrai_r (struct racional r1, struct racional r2)
{
  if (!(valido_r(r1)))
    return r1;
  if (!(valido_r(r2)))
    return r2;
  
  long num, den;
  /* denominadores diferentes calcula o mmc primeiro */
  if (r1.den != r2.den)
  {
    den = mmc(r1.den, r2.den);
    num = ((den/r1.den)*r1.num) - 
          ((den/r2.den)*r2.num);
    return cria_r(num, den);
  }

  /* denominadores iguais apenas subtrai os numeradores */
  num = r1.num - r2.num;
  den = r1.den;
  return cria_r(num, den);
}

/* Retorna a multiplicação dos racionais r1 e r2.
 * se r1 ou r2 for inválido, o resultado deve ser inválido */
struct racional multiplica_r (struct racional r1, struct racional r2)
{
  if (!(valido_r(r1)))
    return r1;
  if (!(valido_r(r2)))
    return r2;

  long num, den;
  num = r1.num * r2.num;
  den = r1.den * r2.den;

  return cria_r(num, den);
}

/* Retorna a divisão dos racionais r1 e r2.
 * se r1 ou r2 for inválido, o resultado deve ser inválido.
 * se o numerador de r2 for 0 retorna um r inválido         */
struct racional divide_r (struct racional r1, struct racional r2)
{
  if (!(valido_r(r1)))
    return r1;
  if (!(valido_r(r2)) || !(r2.num))
    return r2;

  long num, den;
  num = r1.num * r2.den;
  den = r1.den * r2.num;

  return cria_r(num, den);
}
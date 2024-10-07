/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include "racional.h"

#define TAM 100

void ler_v (struct racional *v, int n)
{
  long num, den;
  int i;
  
  for (i = 0; i < n; i++)
  {
    scanf("%ld %ld", &num, &den);
    v[i].num = num;
    v[i].den = den;
  }
}

void imprimi_v (struct racional v[], int n)
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

void elimina_NaN (struct racional *v, int *n)
{
  int i;
  int j = 0;
  
  for (i = 0; i < *n; i++)
  {
    if (v[i].den != 0)
    {
      v[j++] = v[i];
    }
  }

  *n = j;
}

void ordenar_v (struct racional *v, int n)
{
  int i, j;
  struct racional temp;
  for (i = 1; i < n; i++)
  {
    temp = v[i];

    for (j = i - 1; j >= 0 && compara_r(v[j], temp) == 1; j--)
        v[j+1] = v[j];

    v[j+1] = temp;
  }
}

void soma_v (struct racional v[], int n)
{
  for (int i = 0; i < n; i++)
  {
    
  }
}


int main ()
{
  struct racional v[TAM];
  int n;
  struct racional sum;

  do
    scanf("%d", &n);
  while (n <= 0 || n >= 100);

  ler_v(v, n);
  imprimi_v(v, n);

  elimina_NaN(v, &n);
  imprimi_v(v, n);

  ordenar_v(v, n);
  imprimi_v(v, n);

  printf("VETOR = ");
  for (int i = 0; i < n-1; i++)
    if (soma_r(v[i], v[i+1], &sum));

  imprime_r(sum);
  printf("\n");

  return 0;
}

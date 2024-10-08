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
  
  for (i = 0; i < *n; i++)
  {
    if (v[i].den == 0)
    {
      v[i] = v[*n];
      (*n)--;
      i--;
    }
  }
}

void ordenar_v (struct racional *v, int n)
{
  int i, j;
  struct racional temp;

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

int main ()
{
  struct racional v[TAM];
  int n;

  do
    scanf("%d", &n);
  while (n <= 0 || n >= 100);

  ler_v(v, n);
  imprimi_v(v, n);

  elimina_NaN(v, &n);
  imprimi_v(v, n);

  ordenar_v(v, n);
  imprimi_v(v, n);

  printf("\n");

  return 0;
}

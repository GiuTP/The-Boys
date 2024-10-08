#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

void ler_v (int *v, int n)
{
  long num;
  int i;
  
  for (i = 0; i < n; i++)
  {
    scanf("%ld", &num);
    v[i]= num;
  }
}

void imprimi_v (int v[], int n)
{
  int i;

  printf("VETOR = ");
  for (i = 0; i < n; i++)
  {
    printf("%d", v[i]);
    printf(" ");
  }
  printf("\n");
}

void buble_sort (int *v, int n)
{
  int i, j;
  int temp;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n - i - 1; j++)
    {
      if (v[j] > v[j+1])
      {
        temp = v[j];
        v[j] = v[j+1];
        v[j+1] = temp;
      }
    }
  }
}

int main(){
    int n;
    int v[100];
    scanf("%d", &n);
    ler_v(v, n);
    imprimi_v(v, n);
    buble_sort(v, n);
    imprimi_v(v ,n);

    return 0;
}
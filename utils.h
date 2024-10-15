#ifndef AUXILIAR
#define AUXILIAR

void ler_v (struct racional **v, int n);

void imprime_v (struct racional **v, int n);

void elimina_NaN (struct racional **v, int *n);

void ordenar_v (struct racional **v, int n);

int soma_v (struct racional **v, int n, struct racional **sum);

#endif 
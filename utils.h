#ifndef AUXILIAR
#define AUXILIAR

/* Ler os elementos de um vetor de n ponteiros */
void ler_v (struct racional **v, int n);

/* Imprime os racionais apontados por um vetor */
void imprime_v (struct racional **v, int n);

/* Remove os elementos inválidos de um vetor
 * Libera a memória alocada para esse r inválido */
void elimina_NaN (struct racional **v, int *n);

/* Ordena um vetor de ponteiros usando o algoritmo do bubble sort */
void ordenar_v (struct racional **v, int n);

/* Soma os elementos de um vetor de racionais
 * Retorna 1 se a operação foi bem sucecida ou
 *         0 se o r somado for inválido e libera a memória alocada de sum */
int soma_v (struct racional **v, int n, struct racional **sum);

#endif 
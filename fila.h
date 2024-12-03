#ifndef FILA
#define FILA

// estrutura de um item da fila
struct item_t
{
  int valor;           // valor do item
  struct item_t *ant;  // item anterior
  struct item_t *prox; // próximo item
};

// estrutura de uma fila
struct fila_t
{
  struct item_t *prim; // primeiro item
  struct item_t *ult;  // último item
  int tamanho;         // número de itens da fila
};

// Cria uma fila vazia.
// Retorno: ponteiro p/ a fila ou NULL em erro.
struct fila_t *fila_cria();

// Remove todos os itens da fila e libera a memória.
// Retorno: NULL.
struct fila_t *fila_destroi(struct fila_t *fl);

// Insere o item na fila respeitando a politica FIFO;
// Retorno: número de itens na fila após a operação ou -1 em erro.
int enqueue(struct fila_t *fl, int item, int pos);

// Retira o item da fila respeitando a politica FIFO.
// Retorno: número de itens na fila após a operação ou -1 em erro.
int dequeue(struct fila_t *fl, int *item, int pos);

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for -1, consulta do fim.
// Retorno: número de itens na fila ou -1 em erro.
int fila_consulta(struct fila_t *fl, int *item, int pos);

// Informa a posição da 1ª ocorrência do valor indicado na fila.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int fila_procura(struct fila_t *fl, int valor);

// Informa o tamanho da fila (o número de itens presentes nela).
// Retorno: número de itens na fila ou -1 em erro.
int fila_tamanho(struct fila_t *fl);

// Imprime o conteúdo da fila do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void fila_imprime(struct fila_t *fl);

#endif

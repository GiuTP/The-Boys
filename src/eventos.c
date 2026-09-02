#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "fprio.h"
#include "fila.h"
#include "entidades.h"

#define N_HABILIDADES 10 /* 10 */
typedef enum
{
    CHEGA = 1,
    ESPERA,
    DESISTE,
    AVISA,
    ENTRA,
    SAI,
    VIAJA,
    MORRE,
    MISSAO,
    FIM
} EventType;

/* Cria eventos inseridos na LEF. Vincula tempo, tipo, heroi, base e missao ao evento */
struct event *cria_evento(int t, int type, int h, int b, int m)
{
    struct event *ev;

    if (!(ev = malloc(sizeof(struct event))))
        return NULL;

    ev->tempo = t;
    ev->tipo = type;
    ev->h_id = h;
    ev->b_id = b;
    ev->m_id = m;

    return ev;
}

/* Distancia euclediana entre dois pontos */
int dist_pts(struct coord p1, struct coord p2)
{
    return (sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));
}

/* Troca o indice a com o indice b de um vetor de bases_m */
void swap(struct bases_m *a, struct bases_m *b)
{
    struct bases_m t;

    t = *a;
    *a = *b;
    *b = t;
}

/* Particiona o vetor entre low e high */
/* Retorna: o indice do pivo apos a particao */
int partition(struct bases_m *bC, int low, int high)
{
    int p;
    int i, j;

    p = bC[high].dist;

    i = low - 1;
    for (j = low; j <= high - 1; j++)
    {
        if (bC[j].dist < p)
        {
            i++;
            swap(&bC[i], &bC[j]);
        }
    }

    swap(&bC[i + 1], &bC[high]);

    return i + 1;
}

/* Algoritmo de ordenacao quick sort recursivo para o vetor de bases_m */
void quick_sort(struct bases_m *bC, int low, int high)
{
    int pivot;

    if (low < high)
    {
        pivot = partition(bC, low, high);

        quick_sort(bC, low, pivot - 1);
        quick_sort(bC, pivot + 1, high);
    }
}

void evento_chega(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int espera;

    /* Recupera heroi e base vinculados ao evento. */
    /* Se repete para todos os eventos, mudando apenas o que esta sendo recuperado. */
    h = &my_world->herois[ev->h_id];
    b = &my_world->bases[ev->b_id];

    /* Incrementa contador de eventos. */
    /* Se repete em todos os eventos tambem. */
    my_world->infos.ev_tratados++;

    /* Atualiza a base do heroi */
    h->base = b->ID;

    if ((b->h_presentes->num < b->lotação) && (b->f_espera->prim == NULL))
        espera = 1;
    else
        espera = (h->paciência) > (10 * fila_tamanho(b->f_espera));

    /* Cria e adiciona evento na LEF e imprime mensagem do evento. */
    if (espera)
    {
        /* Criando evento e adicionando na LEF. */
        new_ev = cria_evento(ev->tempo, ESPERA, h->ID, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n",
               ev->tempo, ev->h_id, ev->b_id, b->h_presentes->num, b->lotação);
    }
    else
    {
        new_ev = cria_evento(ev->tempo, DESISTE, h->ID, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n",
               ev->tempo, ev->h_id, ev->b_id, cjto_card(b->h_presentes), b->lotação);
    }
}

void evento_espera(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int q_tam;

    h = &my_world->herois[ev->h_id];
    b = &my_world->bases[ev->b_id];

    my_world->infos.ev_tratados++;

    /* Insere heroi na fila de espera da base. */
    enqueue(b->f_espera, ev->h_id, -1);

    /* Usado para atualizar o f_max (maior tamanho da fila). */
    q_tam = fila_tamanho(b->f_espera);
    if (q_tam > b->f_max)
        b->f_max = q_tam;

    /* Cria e adiciona evento na LEF e imprime mensagem do evento. */
    new_ev = cria_evento(ev->tempo, AVISA, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
           ev->tempo, h->ID, b->ID, q_tam - 1);
}

void evento_desiste(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int new_base;

    h = &my_world->herois[ev->h_id];
    b = &my_world->bases[ev->b_id];

    my_world->infos.ev_tratados++;

    /* Sorteio da nova base para o heroi ir. */
    new_base = aleat(0, my_world->num_bases - 1);

    /* Cria e adiciona evento na LEF e imprime mensagem do evento. */
    new_ev = cria_evento(ev->tempo, VIAJA, h->ID, new_base, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: DESIST HEROI %2d BASE %d\n",
           ev->tempo, h->ID, b->ID);
}

void evento_avisa(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    int h;
    struct base *b;
    struct event *new_ev;

    b = &my_world->bases[ev->b_id];

    my_world->infos.ev_tratados++;

    /* Mostra quantos herois estao na fila de espera. */
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ",
           ev->tempo, ev->b_id, b->h_presentes->num, b->lotação);
    fila_imprime(b->f_espera);
    printf(" ]\n");

    /* Começar a inserir herois na base enquanto for possivel. */
    while ((b->h_presentes->num < b->lotação) && (b->f_espera->prim != NULL))
    {
        dequeue(b->f_espera, &h, 0);
        cjto_insere(b->h_presentes, h);

        /* Cria e adiciona evento na LEF e imprime mensagem do evento. */
        new_ev = cria_evento(ev->tempo, ENTRA, h, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->b_id, h);
    }
}

void evento_entra(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int tpb;

    h = &my_world->herois[ev->h_id];
    b = &my_world->bases[ev->b_id];

    my_world->infos.ev_tratados++;

    /* Calculo na permanencia do heroi na base */
    tpb = 15 + h->paciência * aleat(1, 20);

    /* Cria e adiciona evento na LEF e imprime mensagem do evento. */
    new_ev = cria_evento(ev->tempo + tpb, SAI, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
           ev->tempo, h->ID, b->ID, b->h_presentes->num, b->lotação, new_ev->tempo);
}

void evento_sai(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int new_base;

    h = &my_world->herois[ev->h_id];
    b = &my_world->bases[ev->b_id];

    my_world->infos.ev_tratados++;

    /* Retira heroi da base e sorteia nova base para ele ir */
    cjto_retira(b->h_presentes, h->ID);
    new_base = aleat(0, my_world->num_bases - 1);

    /* Cria e adiciona eventos na LEF e imprime mensagem do evento:
     * - Primeiro o evento VIAJA;
     * - Depois o evento AVISA;
     * - Mensagem padrao de evento SAI.                             */
    new_ev = cria_evento(ev->tempo, VIAJA, h->ID, new_base, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    new_ev = cria_evento(ev->tempo, AVISA, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
           ev->tempo, h->ID, b->ID, b->h_presentes->num, b->lotação);
}

void evento_viaja(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b_current, *b_next;
    struct event *new_ev;
    int dist, tempo;

    h = &my_world->herois[ev->h_id];
    b_current = &my_world->bases[h->base]; /* base atual */
    b_next = &my_world->bases[ev->b_id];   /* base de destino */

    my_world->infos.ev_tratados++;

    /* Calculo da distância e tempo de viagem. */
    dist = dist_pts(b_current->local, b_next->local);
    tempo = dist / h->velocidade;

    /* Cria e adiciona evento na LEF e imprime mensagem do evento. */
    new_ev = cria_evento(ev->tempo + tempo, CHEGA, h->ID, b_next->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
           ev->tempo, h->ID, b_current->ID, b_next->ID, dist, h->velocidade, new_ev->tempo);
}

void evento_morre(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct mission *m;
    struct event *new_ev;

    h = &my_world->herois[ev->h_id];
    b = &my_world->bases[ev->b_id];
    m = &my_world->missões[ev->m_id];

    /* Atualiza o contador de eventos e mortes. */
    my_world->infos.ev_tratados++;
    my_world->infos.total_mortes++;

    /* Retira heroi da base pois morreu e atualiza o status dele */
    cjto_retira(b->h_presentes, h->ID);
    h->status = 0; /* Status de morto */

    /* Cria e adiciona evento na LEF e imprime mensagem do evento. */
    new_ev = cria_evento(ev->tempo, AVISA, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", ev->tempo, h->ID, m->ID);
}

void evento_missao(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct bases_m *bCand;
    struct cjto_t *union_skills, *us_temp;
    struct mission *m;
    struct event *new_ev;
    int i, j, bmp, risk;

    /* Vetor auxiliar para armazenar as bases candidatas a missao. */
    if (!(bCand = malloc(my_world->num_bases * sizeof(struct bases_m))))
        return;

    /* Recupera missao vinculada ao evento */
    m = &my_world->missões[ev->m_id];

    my_world->infos.ev_tratados++;

    /* Verifica se tentativa da missao eh maior que o maximo atual */
    if (m->tent > my_world->infos.tent_max_missão)
        my_world->infos.tent_max_missão = m->tent;

    /* Imprime habilidades necessarias para a missao */
    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", ev->tempo, ev->m_id, m->tent);
    cjto_imprime(m->hab_necessarias);
    printf(" ]\n");

    /* --------- Primeira parte - inserir no vetor auxiliar todas as bases do mundo --------- */
    /* Insere no vetor bCand todas as bases do mundo */
    for (i = 0; i < my_world->num_bases; i++)
    {
        b = &my_world->bases[i];

        bCand[i].b_id = i;
        bCand[i].dist = dist_pts(m->local, b->local);
        bCand[i].h_id = cjto_copia(b->h_presentes);

        union_skills = cjto_cria(N_HABILIDADES);
        for (j = 0; j < b->h_presentes->cap; j++)
        {
            if (cjto_pertence(b->h_presentes, j))
            {
                h = &my_world->herois[j];
                us_temp = cjto_uniao(union_skills, h->habilidades);
                cjto_destroi(union_skills);
                union_skills = us_temp;
            }
        }
        bCand[i].União = cjto_copia(union_skills);
        cjto_destroi(union_skills);
    }
    /* --------- Primeira parte - inserir no vetor auxiliar todas as bases do mundo --------- */

    /* Ordena em ordem crescente de distancia as bases */
    quick_sort(bCand, 0, my_world->num_bases - 1);

    /* --------- Segunda parte - escolher a base apta mais proxima --------- */
    /* Começa supondo que nao existe base para a missao */
    bmp = -1;
    for (i = 0; i < my_world->num_bases; i++)
    {
        b = &my_world->bases[bCand[i].b_id];

        /* Imprime informacoes da base i analisada */
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", ev->tempo, m->ID, b->ID, bCand[i].dist);
        cjto_imprime(b->h_presentes);
        printf(" ]\n");

        /* Imprime todos os herois presentes na base i */
        for (j = 0; j < b->h_presentes->cap; j++)
        {
            if (cjto_pertence(b->h_presentes, j))
            {
                printf("%6d: MISSAO %d HAB HEROI %2d: [ ", ev->tempo, m->ID, j);
                cjto_imprime(my_world->herois[j].habilidades);
                printf(" ]\n");
            }
        }

        /* Imprime a uniao das habilidades dos herois */
        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", ev->tempo, m->ID, b->ID);
        cjto_imprime(bCand[i].União);
        printf(" ]\n");

        /* O bpm sera atribuido a base mais proxima e que contem os herois necessarios apenas uma vez */
        if ((cjto_contem(bCand[i].União, m->hab_necessarias)) && (bmp == -1) && (cjto_card(bCand[i].União) != 0))
            bmp = i;
    }
    /* --------- Segunda parte - escolher a base apta mais proxima --------- */

    /* --------- Terceira parte - verificar se a missao foi cumprida --------- */
    /* Verifica se foi encontrado alguma base para a missao */
    if (bmp != -1)
    {
        /* Recupera a base apta para a missao */
        b = &my_world->bases[bCand[bmp].b_id];

        /* Incrementa a qtd. de missoes cumpridas e o a qtd que a base b participou de missoes */
        b->part_missão++;
        my_world->infos.comp_missões++;

        /* Imprime mensagem de missao cumprida e o porque foi cumprida */
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", ev->tempo, m->ID, b->ID);
        cjto_imprime(bCand[bmp].União);
        printf(" ]\n");

        /* Para cada heroi calcula se ele morreu na missao ou nao */
        for (i = 0; i < b->h_presentes->cap; i++)
        {
            if (cjto_pertence(b->h_presentes, i))
            {
                h = &my_world->herois[i];
                risk = m->perigo / (h->paciência + h->xp + 1.0);

                /* Caso morreu, adiciona o evento MORRE na LEF */
                if (risk > aleat(0, 30))
                {
                    new_ev = cria_evento(ev->tempo, MORRE, h->ID, b->ID, m->ID);
                    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);
                }
                /* Caso nao, incrementa o xp do heroi */
                else
                {
                    h->xp++;
                }
            }
        }
    }
    /* Caso nao existir bmp para a missao */
    else
    {
        /* Incrementa o numero de tentativas da missao m */
        m->tent++;

        /* Adia a missao para o proximo dia e insere na LEF e imprime mensagem */
        new_ev = cria_evento(ev->tempo + 24 * 60, MISSAO, -1, -1, ev->m_id);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        printf("%6d: MISSAO %d IMPOSSIVEL\n", ev->tempo, ev->m_id);
    }
    /* --------- Terceira parte - verificar se a missao foi cumprida --------- */

    /* --------- Quarta parte - limpar memoria --------- */
    /* Libera toda a memoria alocada para bCand */
    for (i = 0; i < my_world->num_bases; i++)
    {
        cjto_destroi(bCand[i].h_id);
        cjto_destroi(bCand[i].União);
    }
    free(bCand);
    /* --------- Quarta parte - limpar memoria --------- */
}

void evento_fim(struct world my_world, struct event *ev)
{
    struct hero h;
    struct base b;
    int i, sum_attempts;

    my_world.infos.ev_tratados++;

    printf("%6d: FIM\n", ev->tempo);

    /* Estatisticas de todos os herois */
    for (i = 0; i < my_world.num_herois; i++)
    {
        h = my_world.herois[i];
        if (h.status)
        {
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [ ", h.ID, h.paciência, h.velocidade, h.xp);
            cjto_imprime(h.habilidades);
            printf(" ]\n");
        }
        else
        {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", h.ID, h.paciência, h.velocidade, h.xp);
            cjto_imprime(h.habilidades);
            printf(" ]\n");
        }
    }

    /* Estatisticas de todas as bases */
    for (i = 0; i < my_world.num_bases; i++)
    {
        b = my_world.bases[i];

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", b.ID, b.lotação, b.f_max, b.part_missão);
    }

    /* Procura o menor max entre as missoes, esse sera o min de tentativas do mundo */
    sum_attempts = 0;
    for (i = 0; i < my_world.num_missões; i++)
    {
        sum_attempts += my_world.missões[i].tent;
        if (my_world.missões[i].tent < my_world.infos.tent_min_missão)
            my_world.infos.tent_min_missão = my_world.missões[i].tent;
    }

    /* Estatisticas gerais da simulacao */
    printf("EVENTOS TRATADOS: %d\n", my_world.infos.ev_tratados);

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n",
           my_world.infos.comp_missões,
           my_world.num_missões,
           my_world.infos.comp_missões * 100.0 / my_world.num_missões);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n",
           my_world.infos.tent_min_missão, my_world.infos.tent_max_missão, 1.0 * sum_attempts / my_world.num_missões);

    printf("TAXA MORTALIDADE: %.1f%%\n", my_world.infos.total_mortes * 100.0 / my_world.num_herois);
}
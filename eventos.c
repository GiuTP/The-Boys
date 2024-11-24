#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "eventos.h"

#define N_HABILIDADES 3 /* 10 */
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
    ev->hero_id = h;
    ev->base_id = b;
    ev->mission_id = m;

    return ev;
}

/* Distancia euclediana entre dois pontos */
int dist_pts(struct coord p1, struct coord p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
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

/* Algoritmo de ordenacao quick sort recursivo */
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
    int wait;

    /* Recupera heroi e base vinculados ao evento */
    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    /* Atualiza base do heroi */
    h->base = b->ID;

    /* Verifica se o heroi esperara ou nao na fila de espera da base */
    if ((b->present_heroes->num < b->capacity) && (b->waiting_queue->prim == NULL))
        wait = 1;
    else
        wait = (h->patience) > (10 * b->waiting_queue->tamanho);

    /* Adicionando evento novo na LEF */
    if (wait)
    {
        /* Adicionando evento */
        new_ev = cria_evento(ev->tempo, ESPERA, h->ID, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        /* Imprimir mensagem */
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n",
               ev->tempo, ev->hero_id, ev->base_id, b->present_heroes->num, b->capacity);
    }
    else
    {
        /* Adicionando evento */
        new_ev = cria_evento(ev->tempo, DESISTE, h->ID, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        /* Imprimir mensagem */
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n",
               ev->tempo, ev->hero_id, ev->base_id, b->present_heroes->num, b->capacity);
    }
}

void evento_espera(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int q_tam;

    /* Recupera heroi e base vinculados ao evento */
    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    /* Insere heroi na fila de espera da base */
    lista_insere(b->waiting_queue, ev->hero_id, -1);

    /* Usado para estatisticas. Verifica se o tamanho da fila atual eh maior que o maior tamanho ate entao */
    q_tam = lista_tamanho(b->waiting_queue);
    if (q_tam > b->max_queue)
        b->max_queue = q_tam;

    /* Adicionando evento */
    new_ev = cria_evento(ev->tempo, AVISA, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    /* Imprimir mensagem */
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
           ev->tempo, h->ID, b->ID, q_tam - 1);
}

void evento_desiste(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int new_base;

    /* Recupera heroi e base vinculados ao evento */
    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    /* Sorteito da nova base para o heroi ir */
    new_base = aleat(0, my_world->total_bases - 1);

    /* Adicionando evento */
    new_ev = cria_evento(ev->tempo, VIAJA, h->ID, new_base, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    /* Imprimir mensagem */
    printf("%6d: DESIST HEROI %2d BASE %d\n",
           ev->tempo, h->ID, b->ID);
}

void evento_avisa(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    int h;
    struct base *b;
    struct event *new_ev;
    struct item_t *aux;

    /* Recupera base vinculada ao evento */
    b = &my_world->bases[ev->base_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    /* Imprimir quantos heroi estao na fila */
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ",
           ev->tempo, ev->base_id, b->present_heroes->num, b->capacity);
    aux = b->waiting_queue->prim;
    while (aux != NULL)
    {
        printf("%2d ", aux->valor);
        aux = aux->prox;
    }
    printf("]\n");

    /* Adicionando herois na base enquanto houver gente na fila e a base nao estiver cheia */
    while ((b->present_heroes->num < b->capacity) && (b->waiting_queue->prim != NULL))
    {
        /* Retira heroi da fila e insere na base */
        lista_retira(b->waiting_queue, &h, 0);
        cjto_insere(b->present_heroes, h);

        /* Adicionando evento */
        new_ev = cria_evento(ev->tempo, ENTRA, h, b->ID, -1);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        /* Imprimir mensagem */
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->base_id, h);
    }
}

void evento_entra(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int tpb;

    /* Recupera heroi e base vinculados ao evento */
    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    /* Calculo na permanencia do heroi na base */
    tpb = 200 + h->patience * aleat(20, 80); /* 15 + paciencia de h * aleat(1,20) */

    /* Adicionando evento */
    new_ev = cria_evento(ev->tempo + tpb, SAI, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    /* Imprimir mensagem */
    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
           ev->tempo, h->ID, b->ID, b->present_heroes->num, b->capacity, new_ev->tempo);
}

void evento_sai(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct event *new_ev;
    int new_base;

    /* Recupera heroi e base vinculados ao evento */
    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    /* Retira heroi da base e sorteia base para ir */
    cjto_retira(b->present_heroes, h->ID);
    new_base = aleat(0, my_world->total_bases - 1);

    /* Adicionando evento */
    new_ev = cria_evento(ev->tempo, VIAJA, h->ID, new_base, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    /* Adicionando evento */
    new_ev = cria_evento(ev->tempo, AVISA, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    /* Imprimir mensagem */
    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
           ev->tempo, h->ID, b->ID, b->present_heroes->num, b->capacity);
}

void evento_viaja(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b_current, *b_next;
    struct event *new_ev;
    int dist, tempo;

    /* Recupera heroi e bases vinculados ao evento */
    h = &my_world->heroes[ev->hero_id];
    b_current = &my_world->bases[h->base];
    b_next = &my_world->bases[ev->base_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    /* Calculo da distância e tempo de viajem */
    dist = dist_pts(b_current->locatization, b_next->locatization);
    tempo = dist / h->speed;

    /* Adicionando evento */
    new_ev = cria_evento(ev->tempo + tempo, CHEGA, h->ID, b_next->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    /* Imprimir mensagem */
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n",
           ev->tempo, h->ID, b_current->ID, b_next->ID, dist, h->speed, new_ev->tempo);
}

void evento_morre(struct fprio_t **lef, struct world *my_world, struct event *ev)
{
    struct hero *h;
    struct base *b;
    struct mission *m;
    struct event *new_ev;

    /* Recupera heroi, base e missao vinculados ao evento */
    h = &my_world->heroes[ev->hero_id];
    b = &my_world->bases[ev->base_id];
    m = &my_world->missions[ev->mission_id];

    /* Incrementa contador de eventos e mortes */
    my_world->infos.events_handled += 1;
    my_world->infos.total_deaths += 1;

    /* Retira heroi da base pois esta morto e atualiza o status dele */
    cjto_retira(b->present_heroes, h->ID);
    h->status = 0; /* Status de morto */

    /* Adicionando evento */
    new_ev = cria_evento(ev->tempo, AVISA, h->ID, b->ID, -1);
    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

    /* Imprimir mensagem */
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

    /* Vetor auxiliar para armazenar as bases candidatas */
    if (!(bCand = malloc(my_world->total_bases * sizeof(struct bases_m))))
        return;

    /* Recupera missao vinculada ao evento */
    m = &my_world->missions[ev->mission_id];

    /* Incrementa contador de eventos */
    my_world->infos.events_handled += 1;

    if (m->attempts > my_world->infos.max_attempts_mission)
        my_world->infos.max_attempts_mission = m->attempts;

    /* Imprime habilidades necessarias para a missao */
    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", ev->tempo, ev->mission_id, m->attempts);
    cjto_imprime(m->skills_needed);
    printf(" ]\n");

    /* Insere no vetor bCand todas as bases do mundo */
    for (i = 0; i < my_world->total_bases; i++)
    {
        b = &my_world->bases[i];

        bCand[i].base_id = i;
        bCand[i].dist = dist_pts(m->localization, b->locatization);
        bCand[i].heroes_id = cjto_copia(b->present_heroes);

        union_skills = cjto_cria(N_HABILIDADES);
        for (j = 0; j < b->present_heroes->cap; j++)
        {
            if (cjto_pertence(b->present_heroes, j))
            {
                h = &my_world->heroes[j];
                us_temp = cjto_uniao(union_skills, h->skills);
                cjto_destroi(union_skills);
                union_skills = us_temp;
            }
        }
        bCand[i].Union = cjto_copia(union_skills);
        cjto_destroi(union_skills);
    }

    /* Ordena em ordem crescente de distancia as bases */
    quick_sort(bCand, 0, my_world->total_bases - 1);

    /* Começa supondo que nao existe base para a missao */
    bmp = -1;
    for (i = 0; i < my_world->total_bases; i++)
    {
        b = &my_world->bases[bCand[i].base_id];

        /* Imprime mensagem da base i analisada */
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", ev->tempo, m->ID, b->ID, bCand[i].dist);
        cjto_imprime(b->present_heroes);
        printf(" ]\n");

        /* Imprime todos os herois presentes na base i */
        for (j = 0; j < b->present_heroes->cap; j++)
        {
            if (cjto_pertence(b->present_heroes, j))
            {
                printf("%6d: MISSAO %d HAB HEROI %2d: [ ", ev->tempo, m->ID, j);
                cjto_imprime(my_world->heroes[j].skills);
                printf(" ]\n");
            }
        }

        /* Imprime a uniao das habilidades dos herois */
        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", ev->tempo, m->ID, b->ID);
        cjto_imprime(bCand[i].Union);
        printf(" ]\n");

        /* O bpm sera atribuido a base mais proxima e que contem os herois necessarios */
        if ((cjto_contem(bCand[i].Union, m->skills_needed)) && (bmp == -1) && (cjto_card(bCand[i].Union) != 0))
            bmp = i;
    }

    /* Verifica se foi encontrado alguma base para a missao */
    if (bmp != -1)
    {
        b = &my_world->bases[bCand[bmp].base_id];

        /* Incrementa o contador de missoes que a base participou e o numero de missoes completas do mundo */
        b->mission_participation += 1;
        my_world->infos.missions_completed += 1;

        /* Imprime mensagem de missao cumprida e o porque foi cumprida */
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", ev->tempo, m->ID, b->ID);
        cjto_imprime(bCand[bmp].Union);
        printf(" ]\n");

        /* Para cada heroi calcula se ele morreu na missao ou nao */
        for (i = 0; i < b->present_heroes->cap; i++)
        {
            if (cjto_pertence(b->present_heroes, i))
            {
                h = &my_world->heroes[i];
                risk = m->danger / (h->patience + h->experience + 1.0); /* risk = m->danger / (h->patience + h->experience + 1.0); */

                /* Caso morreu, adiciona o evento MORRE na LEF */
                if (risk > aleat(0, 30))
                {
                    new_ev = cria_evento(ev->tempo, MORRE, h->ID, b->ID, m->ID);
                    fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);
                }
                /* Caso nao, incrementa o xp do heroi */
                else
                {
                    h->experience += 1;
                }
            }
        }
    }
    /* Caso nao existir bmp para a missao */
    else
    {
        /* Incrementa o numero de tentativas da missao m */
        m->attempts += 1;

        /* Adia a missao para o proximo dia e insere na LEF */
        new_ev = cria_evento(ev->tempo + 24 * 60, MISSAO, -1, -1, ev->mission_id);
        fprio_insere(*lef, new_ev, new_ev->tipo, new_ev->tempo);

        /* Imprime mensagem */
        printf("%6d: MISSAO %d IMPOSSIVEL\n", ev->tempo, ev->mission_id);
    }

    /* Libera toda a memoria alocada para bCand */
    for (i = 0; i < my_world->total_bases; i++)
    {
        cjto_destroi(bCand[i].heroes_id);
        cjto_destroi(bCand[i].Union);
    }
    free(bCand);
}

void evento_fim(struct world my_world, struct event *ev)
{
    struct hero h;
    struct base b;
    int i, sum_attempts;

    /* Incrementa contador de eventos */
    my_world.infos.events_handled += 1;

    printf("%6d: FIM\n", ev->tempo);

    /* Estatisticas de todos os herois */
    for (i = 0; i < my_world.total_heroes; i++)
    {
        h = my_world.heroes[i];
        if (h.status)
        {
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [ ", h.ID, h.patience, h.speed, h.experience);
            cjto_imprime(h.skills);
            printf(" ]\n");
        }
        else
        {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", h.ID, h.patience, h.speed, h.experience);
            cjto_imprime(h.skills);
            printf(" ]\n");
        }
    }

    /* Estatisticas de todas as bases */
    for (i = 0; i < my_world.total_bases; i++)
    {
        b = my_world.bases[i];

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", b.ID, b.capacity, b.max_queue, b.mission_participation);
    }

    /* Procura o menor max entre as missoes, esse sera o min de tentativas do mundo */
    sum_attempts = 0;
    for (i = 0; i < my_world.total_missions; i++)
    {
        sum_attempts += my_world.missions[i].attempts;
        if (my_world.missions[i].attempts < my_world.infos.min_attempts_mission)
            my_world.infos.min_attempts_mission = my_world.missions[i].attempts;
    }

    /* Estatisticas gerais da simulacao */
    printf("EVENTOS TRATADOS: %d\n", my_world.infos.events_handled);

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n",
           my_world.infos.missions_completed,
           my_world.total_missions,
           my_world.infos.missions_completed * 100.0 / my_world.total_missions);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n",
           my_world.infos.min_attempts_mission, my_world.infos.max_attempts_mission, 1.0 * sum_attempts / my_world.total_missions);

    printf("TAXA MORTALIDADE: %.1f%%\n", my_world.infos.total_deaths * 100.0 / my_world.total_heroes);
}
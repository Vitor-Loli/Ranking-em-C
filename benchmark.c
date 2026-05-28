#include "benchmark.h"
#include "avl.h"
#include "rbtree.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INTERVALO_PONTUACAO 200000

static void embaralhar(int *vetor, int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j   = rand() % (i + 1);
        int tmp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = tmp;
    }
}

static void pontuacoes_unicas(int *vetor, int n) {
    int tam_conjunto = (n <= INTERVALO_PONTUACAO) ? INTERVALO_PONTUACAO : n;
    int *conjunto    = malloc(tam_conjunto * sizeof(int));
    for (int i = 0; i < tam_conjunto; i++) conjunto[i] = i + 1;
    embaralhar(conjunto, tam_conjunto);
    for (int i = 0; i < n; i++) vetor[i] = conjunto[i];
    free(conjunto);
}

void executar_benchmark(int n_operacoes) {
    printf("\n\n       BENCHMARK: %6d operacoes\n", n_operacoes);

    srand(42);

    int *pontuacoes = malloc(n_operacoes * sizeof(int));
    pontuacoes_unicas(pontuacoes, n_operacoes);

    int n_remocoes = n_operacoes / 2;
    char buf_nome[TAM_NOME];

    /*  AVL  */
    ArvoreAVL *avl = avl_criar();

    clock_t t0 = clock();
    for (int i = 0; i < n_operacoes; i++) {
        snprintf(buf_nome, TAM_NOME, "J%d", i);
        avl_inserir(avl, pontuacoes[i], buf_nome);
    }
    for (int i = 0; i < n_remocoes; i++) {
        avl_remover(avl, pontuacoes[i]);
    }
    clock_t t1 = clock();

    double tempo_avl = (double)(t1 - t0) / CLOCKS_PER_SEC;
    int    rot_avl   = avl->cont_rotacoes;
    int    tam_avl   = avl->tamanho;
    avl_destruir(avl);

    /*  Rubro-Negra  */
    ArvoreRN *rn = rn_criar();

    clock_t t2 = clock();
    for (int i = 0; i < n_operacoes; i++) {
        snprintf(buf_nome, TAM_NOME, "J%d", i);
        rn_inserir(rn, pontuacoes[i], buf_nome);
    }
    for (int i = 0; i < n_remocoes; i++) {
        rn_remover(rn, pontuacoes[i]);
    }
    clock_t t3 = clock();

    double tempo_rn = (double)(t3 - t2) / CLOCKS_PER_SEC;
    int    rot_rn   = rn->cont_rotacoes;
    int    tam_rn   = rn->tamanho;
    rn_destruir(rn);

    free(pontuacoes);

    /* Relatorio  */
    printf("\n%-28s %10s %10s\n", "", "AVL", "Rubro-Neg.");
    printf("%-28s %10s %10s\n",
           "----------------------------", "----------", "----------");
    printf("%-28s %10d %10d\n", "Insercoes",       n_operacoes, n_operacoes);
    printf("%-28s %10d %10d\n", "Remocoes",        n_remocoes,  n_remocoes);
    printf("%-28s %10d %10d\n", "Nos restantes",   tam_avl,     tam_rn);
    printf("%-28s %10d %10d\n", "Rotacoes totais", rot_avl,     rot_rn);
    printf("%-28s %10.4f %10.4f\n", "Tempo (s)",   tempo_avl,   tempo_rn);

    printf("\n Comparacao\n");
    if (rot_rn > 0)
        printf("  AVL realizou %.1f%% das rotacoes da RN\n",
               100.0 * rot_avl / rot_rn);
    if (tempo_rn > 0.0)
        printf("  AVL levou    %.1f%% do tempo da RN\n",
               100.0 * tempo_avl / tempo_rn);
}

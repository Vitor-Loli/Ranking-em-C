#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "rbtree.h"
#include "benchmark.h"

/* Impressao do top-10 */

static void imprimir_top10_avl(ArvoreAVL *arvore) {
    NoAVL *melhores[10];
    int contagem;
    avl_top10(arvore, melhores, &contagem);
    printf("\n  Rank  %-22s  Pontos\n", "Jogador");
    printf("  ----  %-22s  -------\n", "----------------------");
    for (int i = 0; i < contagem; i++)
        printf("  %3d.  %-22s  %d\n", i + 1, melhores[i]->nome, melhores[i]->pontuacao);
}

static void imprimir_top10_rn(ArvoreRN *arvore) {
    NoRN *melhores[10];
    int contagem;
    rn_top10(arvore, melhores, &contagem);
    printf("\n  Rank  %-22s  Pontos\n", "Jogador");
    printf("  ----  %-22s  -------\n", "----------------------");
    for (int i = 0; i < contagem; i++)
        printf("  %3d.  %-22s  %d\n", i + 1, melhores[i]->nome, melhores[i]->pontuacao);
}

/* Menu modo manual */

static void imprimir_menu(void) {
    printf("\n--- MENU ---\n");
    printf("  1. Adicionar jogador\n");
    printf("  2. Remover jogador (por pontuacao)\n");
    printf("  3. Buscar jogador (por pontuacao)\n");
    printf("  4. Ver top-10\n");
    printf("  0. Sair\n");
    printf("Opcao: ");
}

static void modo_interativo(void) {
    printf("\n(As arvores antigas foram limpas e usadas apenas para realizar a comparacao proposta na atividade)\n");
    printf("\n========================================\n");
    printf("       MODO MANUAL \n");
    printf("========================================\n");

    ArvoreAVL *avl = avl_criar();
    ArvoreRN  *rn  = rn_criar();

    int opcao;
    do {
        imprimir_menu();
        if (scanf("%d", &opcao) != 1) break;
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (opcao == 1) {
            char nome[TAM_NOME];
            int  pontos;
            printf("Nome do jogador : ");
            fgets(nome, TAM_NOME, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            printf("Pontuacao       : ");
            scanf("%d", &pontos);
            while ((c = getchar()) != '\n' && c != EOF);

            avl_inserir(avl, pontos, nome);
            rn_inserir (rn,  pontos, nome);
            printf(">> %s (%d pts) inserido.\n", nome, pontos);

        } else if (opcao == 2) {
            int pontos;
            printf("Pontuacao a remover: ");
            scanf("%d", &pontos);
            while ((c = getchar()) != '\n' && c != EOF);

            NoAVL *encontrado = avl_buscar(avl, pontos);
            if (encontrado) {
                printf(">> Removendo %s (%d pts).\n", encontrado->nome, pontos);
                avl_remover(avl, pontos);
                rn_remover (rn,  pontos);
            } else {
                printf(">> Pontuacao %d nao encontrada.\n", pontos);
            }

        } else if (opcao == 3) {
            int pontos;
            printf("Pontuacao a buscar: ");
            scanf("%d", &pontos);
            while ((c = getchar()) != '\n' && c != EOF);

            NoAVL *encontrado = avl_buscar(avl, pontos);
            if (encontrado)
                printf(">> Encontrado: %s (%d pts)\n",
                       encontrado->nome, encontrado->pontuacao);
            else
                printf(">> Pontuacao %d nao encontrada.\n", pontos);

        } else if (opcao == 4) {
            printf("\n[AVL]");
            imprimir_top10_avl(avl);
            printf("  Rotacoes AVL: %d | Nos: %d\n",
                   avl->cont_rotacoes, avl->tamanho);

            printf("\n[Rubro-Negra]");
            imprimir_top10_rn(rn);
            printf("  Rotacoes RN : %d | Nos: %d\n",
                   rn->cont_rotacoes, rn->tamanho);

        } else if (opcao != 0) {
            printf(">> Opcao invalida.\n");
        }

    } while (opcao != 0);

    avl_destruir(avl);
    rn_destruir(rn);
    printf("Encerrando modo interativo.\n");
}


int main(void) {
    printf("       SISTEMA DE RANKING EM TEMPO REAL\n");
    printf("    AVL  vs  Arvore Rubro-Negra  -  Comparativo\n");

    executar_benchmark(10000);
    modo_interativo();

    return 0;
}

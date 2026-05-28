#ifndef RBTREE_H
#define RBTREE_H

#include "player.h"

typedef enum { RN_VERMELHO = 0, RN_PRETO = 1 } CorRN;

typedef struct NoRN {
    int    pontuacao;
    char   nome[TAM_NOME];
    CorRN  cor;
    struct NoRN *esq;
    struct NoRN *dir;
    struct NoRN *pai;
} NoRN;

typedef struct {
    NoRN *raiz;
    NoRN *nulo;
    int   cont_rotacoes;
    int   tamanho;
} ArvoreRN;

ArvoreRN *rn_criar    (void);
void      rn_destruir (ArvoreRN *arvore);

void      rn_inserir  (ArvoreRN *arvore, int pontuacao, const char *nome);
void      rn_remover  (ArvoreRN *arvore, int pontuacao);
NoRN     *rn_buscar   (ArvoreRN *arvore, int pontuacao);

void      rn_top10    (ArvoreRN *arvore, NoRN **melhores, int *contagem);

#endif /* RBTREE_H */

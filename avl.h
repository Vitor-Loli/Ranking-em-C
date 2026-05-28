#ifndef AVL_H
#define AVL_H

#include "player.h"

typedef struct NoAVL {
    int   pontuacao;
    char  nome[TAM_NOME];
    int   altura;
    struct NoAVL *esq;
    struct NoAVL *dir;
} NoAVL;

typedef struct {
    NoAVL *raiz;
    int    cont_rotacoes;
    int    tamanho;
} ArvoreAVL;

ArvoreAVL *avl_criar    (void);
void       avl_destruir (ArvoreAVL *arvore);

void       avl_inserir  (ArvoreAVL *arvore, int pontuacao, const char *nome);
void       avl_remover  (ArvoreAVL *arvore, int pontuacao);
NoAVL     *avl_buscar   (ArvoreAVL *arvore, int pontuacao);

void       avl_top10    (ArvoreAVL *arvore, NoAVL **melhores, int *contagem);

#endif /* AVL_H */

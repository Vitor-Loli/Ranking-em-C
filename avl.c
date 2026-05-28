#include "avl.h"
#include <stdlib.h>
#include <string.h>

static int altura_no(const NoAVL *no) {
    return no ? no->altura : 0;
}

static int maximo_int(int a, int b) {
    return a > b ? a : b;
}

static void atualizar_altura(NoAVL *no) {
    no->altura = 1 + maximo_int(altura_no(no->esq), altura_no(no->dir));
}

static int fator_balanceamento(const NoAVL *no) {
    return no ? altura_no(no->esq) - altura_no(no->dir) : 0;
}

static NoAVL *novo_no(int pontuacao, const char *nome) {
    NoAVL *no     = malloc(sizeof(NoAVL));
    no->pontuacao = pontuacao;
    no->altura    = 1;
    no->esq       = no->dir = NULL;
    strncpy(no->nome, nome, TAM_NOME - 1);
    no->nome[TAM_NOME - 1] = '\0';
    return no;
}


static NoAVL *rotacionar_direita(NoAVL *y, int *cont_rot) {
    NoAVL *x  = y->esq;
    NoAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    atualizar_altura(y);
    atualizar_altura(x);

    (*cont_rot)++;
    return x;
}

static NoAVL *rotacionar_esquerda(NoAVL *x, int *cont_rot) {
    NoAVL *y  = x->dir;
    NoAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    atualizar_altura(x);
    atualizar_altura(y);

    (*cont_rot)++;
    return y;
}


static NoAVL *balancear(NoAVL *no, int *cont_rot) {
    atualizar_altura(no);
    int fb = fator_balanceamento(no);

    /* Esquerda-Esquerda: rotacao simples direita */
    if (fb > 1 && fator_balanceamento(no->esq) >= 0)
        return rotacionar_direita(no, cont_rot);

    /* Esquerda-Direita: rotacao dupla */
    if (fb > 1 && fator_balanceamento(no->esq) < 0) {
        no->esq = rotacionar_esquerda(no->esq, cont_rot);
        return rotacionar_direita(no, cont_rot);
    }

    /* Direita-Direita: rotacao simples esquerda */
    if (fb < -1 && fator_balanceamento(no->dir) <= 0)
        return rotacionar_esquerda(no, cont_rot);

    /* Direita-Esquerda: rotacao dupla */
    if (fb < -1 && fator_balanceamento(no->dir) > 0) {
        no->dir = rotacionar_direita(no->dir, cont_rot);
        return rotacionar_esquerda(no, cont_rot);
    }

    return no;
}


static NoAVL *inserir_rec(NoAVL *no, int pontuacao, const char *nome,
                           int *cont_rot) {
    if (!no) return novo_no(pontuacao, nome);

    if (pontuacao < no->pontuacao)
        no->esq = inserir_rec(no->esq, pontuacao, nome, cont_rot);
    else if (pontuacao > no->pontuacao)
        no->dir = inserir_rec(no->dir, pontuacao, nome, cont_rot);
    else {
        strncpy(no->nome, nome, TAM_NOME - 1);
        return no;
    }

    return balancear(no, cont_rot);
}


static NoAVL *no_minimo(NoAVL *no) {
    while (no->esq) no = no->esq;
    return no;
}

static NoAVL *remover_rec(NoAVL *no, int pontuacao, int *cont_rot) {
    if (!no) return NULL;

    if (pontuacao < no->pontuacao) {
        no->esq = remover_rec(no->esq, pontuacao, cont_rot);
    } else if (pontuacao > no->pontuacao) {
        no->dir = remover_rec(no->dir, pontuacao, cont_rot);
    } else {
        if (!no->esq || !no->dir) {
            NoAVL *filho = no->esq ? no->esq : no->dir;
            free(no);
            return filho;
        }
        NoAVL *suc    = no_minimo(no->dir);
        no->pontuacao = suc->pontuacao;
        strncpy(no->nome, suc->nome, TAM_NOME - 1);
        no->dir = remover_rec(no->dir, suc->pontuacao, cont_rot);
    }

    return balancear(no, cont_rot);
}


static void destruir_rec(NoAVL *no) {
    if (!no) return;
    destruir_rec(no->esq);
    destruir_rec(no->dir);
    free(no);
}

static void top10_rec(NoAVL *no, NoAVL **resultado,
                      int *contagem, int max_cont) {
    if (!no || *contagem >= max_cont) return;
    top10_rec(no->dir, resultado, contagem, max_cont);
    if (*contagem < max_cont) resultado[(*contagem)++] = no;
    top10_rec(no->esq, resultado, contagem, max_cont);
}

ArvoreAVL *avl_criar(void) {
    ArvoreAVL *arvore     = malloc(sizeof(ArvoreAVL));
    arvore->raiz          = NULL;
    arvore->cont_rotacoes = 0;
    arvore->tamanho       = 0;
    return arvore;
}

void avl_destruir(ArvoreAVL *arvore) {
    destruir_rec(arvore->raiz);
    free(arvore);
}

void avl_inserir(ArvoreAVL *arvore, int pontuacao, const char *nome) {
    int existe = avl_buscar(arvore, pontuacao) != NULL;
    arvore->raiz = inserir_rec(arvore->raiz, pontuacao, nome,
                               &arvore->cont_rotacoes);
    if (!existe) arvore->tamanho++;
}

void avl_remover(ArvoreAVL *arvore, int pontuacao) {
    if (!avl_buscar(arvore, pontuacao)) return;
    arvore->raiz = remover_rec(arvore->raiz, pontuacao,
                               &arvore->cont_rotacoes);
    arvore->tamanho--;
}

NoAVL *avl_buscar(ArvoreAVL *arvore, int pontuacao) {
    NoAVL *atual = arvore->raiz;
    while (atual) {
        if (pontuacao == atual->pontuacao) return atual;
        atual = (pontuacao < atual->pontuacao) ? atual->esq : atual->dir;
    }
    return NULL;
}

void avl_top10(ArvoreAVL *arvore, NoAVL **melhores, int *contagem) {
    *contagem = 0;
    top10_rec(arvore->raiz, melhores, contagem, 10);
}

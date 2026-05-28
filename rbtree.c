#include "rbtree.h"
#include <stdlib.h>
#include <string.h>


static void rotacionar_esquerda(ArvoreRN *arvore, NoRN *x) {
    NoRN *y = x->dir;

    x->dir = y->esq;
    if (y->esq != arvore->nulo)
        y->esq->pai = x;

    y->pai = x->pai;
    if (x->pai == arvore->nulo)
        arvore->raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;

    y->esq  = x;
    x->pai  = y;

    arvore->cont_rotacoes++;
}

static void rotacionar_direita(ArvoreRN *arvore, NoRN *y) {
    NoRN *x = y->esq;

    y->esq = x->dir;
    if (x->dir != arvore->nulo)
        x->dir->pai = y;

    x->pai = y->pai;
    if (y->pai == arvore->nulo)
        arvore->raiz = x;
    else if (y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;

    x->dir  = y;
    y->pai  = x;

    arvore->cont_rotacoes++;
}


static void corrigir_insercao(ArvoreRN *arvore, NoRN *z) {
    while (z->pai->cor == RN_VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            NoRN *tio = z->pai->pai->dir;

            if (tio->cor == RN_VERMELHO) {
                /* Caso 1 */
                z->pai->cor       = RN_PRETO;
                tio->cor          = RN_PRETO;
                z->pai->pai->cor  = RN_VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    /* Caso 2 */
                    z = z->pai;
                    rotacionar_esquerda(arvore, z);
                }
                /* Caso 3 */
                z->pai->cor      = RN_PRETO;
                z->pai->pai->cor = RN_VERMELHO;
                rotacionar_direita(arvore, z->pai->pai);
            }
        } else {
            NoRN *tio = z->pai->pai->esq;

            if (tio->cor == RN_VERMELHO) {
                /* Caso 1 espelho */
                z->pai->cor      = RN_PRETO;
                tio->cor         = RN_PRETO;
                z->pai->pai->cor = RN_VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    /* Caso 2 espelho */
                    z = z->pai;
                    rotacionar_direita(arvore, z);
                }
                /* Caso 3 espelho */
                z->pai->cor      = RN_PRETO;
                z->pai->pai->cor = RN_VERMELHO;
                rotacionar_esquerda(arvore, z->pai->pai);
            }
        }
    }
    arvore->raiz->cor = RN_PRETO;
}

static void transplantar(ArvoreRN *arvore, NoRN *u, NoRN *v) {
    if (u->pai == arvore->nulo)
        arvore->raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;
    v->pai = u->pai;
}


static NoRN *minimo_arvore(ArvoreRN *arvore, NoRN *x) {
    while (x->esq != arvore->nulo) x = x->esq;
    return x;
}


static void corrigir_remocao(ArvoreRN *arvore, NoRN *x) {
    while (x != arvore->raiz && x->cor == RN_PRETO) {
        if (x == x->pai->esq) {
            NoRN *irmao = x->pai->dir;

            if (irmao->cor == RN_VERMELHO) {
                /* Caso 1 */
                irmao->cor   = RN_PRETO;
                x->pai->cor  = RN_VERMELHO;
                rotacionar_esquerda(arvore, x->pai);
                irmao = x->pai->dir;
            }

            if (irmao->esq->cor == RN_PRETO && irmao->dir->cor == RN_PRETO) {
                /* Caso 2 */
                irmao->cor = RN_VERMELHO;
                x = x->pai;
            } else {
                if (irmao->dir->cor == RN_PRETO) {
                    /* Caso 3 */
                    irmao->esq->cor = RN_PRETO;
                    irmao->cor      = RN_VERMELHO;
                    rotacionar_direita(arvore, irmao);
                    irmao = x->pai->dir;
                }
                /* Caso 4 */
                irmao->cor      = x->pai->cor;
                x->pai->cor     = RN_PRETO;
                irmao->dir->cor = RN_PRETO;
                rotacionar_esquerda(arvore, x->pai);
                x = arvore->raiz;
            }
        } else {
            NoRN *irmao = x->pai->esq;

            if (irmao->cor == RN_VERMELHO) {
                /* Caso 1 espelho */
                irmao->cor   = RN_PRETO;
                x->pai->cor  = RN_VERMELHO;
                rotacionar_direita(arvore, x->pai);
                irmao = x->pai->esq;
            }

            if (irmao->dir->cor == RN_PRETO && irmao->esq->cor == RN_PRETO) {
                /* Caso 2 espelho */
                irmao->cor = RN_VERMELHO;
                x = x->pai;
            } else {
                if (irmao->esq->cor == RN_PRETO) {
                    /* Caso 3 espelho */
                    irmao->dir->cor = RN_PRETO;
                    irmao->cor      = RN_VERMELHO;
                    rotacionar_esquerda(arvore, irmao);
                    irmao = x->pai->esq;
                }
                /* Caso 4 espelho */
                irmao->cor      = x->pai->cor;
                x->pai->cor     = RN_PRETO;
                irmao->esq->cor = RN_PRETO;
                rotacionar_direita(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    x->cor = RN_PRETO;
}


static void destruir_rec(ArvoreRN *arvore, NoRN *no) {
    if (no == arvore->nulo) return;
    destruir_rec(arvore, no->esq);
    destruir_rec(arvore, no->dir);
    free(no);
}

static void top10_rec(ArvoreRN *arvore, NoRN *no,
                      NoRN **resultado, int *contagem, int max_cont) {
    if (no == arvore->nulo || *contagem >= max_cont) return;
    top10_rec(arvore, no->dir, resultado, contagem, max_cont);
    if (*contagem < max_cont) resultado[(*contagem)++] = no;
    top10_rec(arvore, no->esq, resultado, contagem, max_cont);
}

ArvoreRN *rn_criar(void) {
    ArvoreRN *arvore = malloc(sizeof(ArvoreRN));

    arvore->nulo            = malloc(sizeof(NoRN));
    arvore->nulo->cor       = RN_PRETO;
    arvore->nulo->esq       = arvore->nulo;
    arvore->nulo->dir       = arvore->nulo;
    arvore->nulo->pai       = arvore->nulo;
    arvore->nulo->pontuacao = 0;
    arvore->nulo->nome[0]   = '\0';

    arvore->raiz          = arvore->nulo;
    arvore->cont_rotacoes = 0;
    arvore->tamanho       = 0;
    return arvore;
}

void rn_destruir(ArvoreRN *arvore) {
    destruir_rec(arvore, arvore->raiz);
    free(arvore->nulo);
    free(arvore);
}

void rn_inserir(ArvoreRN *arvore, int pontuacao, const char *nome) {
    NoRN *existente = rn_buscar(arvore, pontuacao);
    if (existente) {
        strncpy(existente->nome, nome, TAM_NOME - 1);
        return;
    }

    NoRN *z       = malloc(sizeof(NoRN));
    z->pontuacao  = pontuacao;
    z->cor        = RN_VERMELHO;
    z->esq        = arvore->nulo;
    z->dir        = arvore->nulo;
    z->pai        = arvore->nulo;
    strncpy(z->nome, nome, TAM_NOME - 1);
    z->nome[TAM_NOME - 1] = '\0';

    NoRN *y = arvore->nulo;
    NoRN *x = arvore->raiz;
    while (x != arvore->nulo) {
        y = x;
        x = (z->pontuacao < x->pontuacao) ? x->esq : x->dir;
    }

    z->pai = y;
    if (y == arvore->nulo)
        arvore->raiz = z;
    else if (z->pontuacao < y->pontuacao)
        y->esq = z;
    else
        y->dir = z;

    corrigir_insercao(arvore, z);
    arvore->tamanho++;
}

void rn_remover(ArvoreRN *arvore, int pontuacao) {
    NoRN *z = rn_buscar(arvore, pontuacao);
    if (!z) return;

    NoRN  *y              = z;
    NoRN  *x;
    CorRN  cor_original_y = y->cor;

    if (z->esq == arvore->nulo) {
        x = z->dir;
        transplantar(arvore, z, z->dir);
    } else if (z->dir == arvore->nulo) {
        x = z->esq;
        transplantar(arvore, z, z->esq);
    } else {
        y              = minimo_arvore(arvore, z->dir);
        cor_original_y = y->cor;
        x              = y->dir;

        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantar(arvore, y, y->dir);
            y->dir      = z->dir;
            y->dir->pai = y;
        }
        transplantar(arvore, z, y);
        y->esq      = z->esq;
        y->esq->pai = y;
        y->cor      = z->cor;
    }

    free(z);
    if (cor_original_y == RN_PRETO)
        corrigir_remocao(arvore, x);

    arvore->tamanho--;
}

NoRN *rn_buscar(ArvoreRN *arvore, int pontuacao) {
    NoRN *atual = arvore->raiz;
    while (atual != arvore->nulo) {
        if (pontuacao == atual->pontuacao) return atual;
        atual = (pontuacao < atual->pontuacao) ? atual->esq : atual->dir;
    }
    return NULL;
}

void rn_top10(ArvoreRN *arvore, NoRN **melhores, int *contagem) {
    *contagem = 0;
    top10_rec(arvore, arvore->raiz, melhores, contagem, 10);
}



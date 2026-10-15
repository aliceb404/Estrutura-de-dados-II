/* QUESTAO 4 - Arvore AVL (insercao, remocao e rotacoes LL, RR, LR, RL) */
#include <stdio.h>
#include <stdlib.h>

/* a) Estrutura do no */
typedef struct no {
    int chave;
    struct no *esq;
    struct no *dir;
    int altura;
} No;

static int rotacoes = 0;       /* conta as rotacoes (para informar se houve alguma) */

/* b) Funcoes basicas (altura de no nulo = -1, de folha = 0) */
int altura(No *no) {
    return (no == NULL) ? -1 : no->altura;
}

static void atualizaAltura(No *no) {
    int he = altura(no->esq), hd = altura(no->dir);
    no->altura = 1 + (he > hd ? he : hd);
}

int fatorBalanceamento(No *no) {
    return (no == NULL) ? 0 : altura(no->esq) - altura(no->dir);
}

/* Rotacao simples a direita (caso LL): o filho esquerdo x sobe e y vira seu filho direito */
No* rotacaoDireita(No *y) {
    No *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualizaAltura(y);         /* y primeiro: agora esta abaixo de x */
    atualizaAltura(x);
    rotacoes++;
    return x;
}

/* Rotacao simples a esquerda (caso RR): o filho direito y sobe e x vira seu filho esquerdo */
No* rotacaoEsquerda(No *x) {
    No *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualizaAltura(x);         /* x primeiro: agora esta abaixo de y */
    atualizaAltura(y);
    rotacoes++;
    return y;
}

/* Rebalanceia um no (usado por inserir e remover) e informa o caso ocorrido */
static No* balancear(No *no) {
    int fb;
    atualizaAltura(no);
    fb = fatorBalanceamento(no);

    if (fb > 1) {                                         /* pesada a esquerda */
        if (fatorBalanceamento(no->esq) >= 0) {
            printf("  >> no %d, FB = %+d: caso LL -> rotacao direita em %d\n", no->chave, fb, no->chave);
            return rotacaoDireita(no);
        }
        printf("  >> no %d, FB = %+d: caso LR -> rotacao esquerda em %d e direita em %d\n",
               no->chave, fb, no->esq->chave, no->chave);
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }
    if (fb < -1) {                                        /* pesada a direita */
        if (fatorBalanceamento(no->dir) <= 0) {
            printf("  >> no %d, FB = %+d: caso RR -> rotacao esquerda em %d\n", no->chave, fb, no->chave);
            return rotacaoEsquerda(no);
        }
        printf("  >> no %d, FB = %+d: caso RL -> rotacao direita em %d e esquerda em %d\n",
               no->chave, fb, no->dir->chave, no->chave);
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }
    return no;
}

static No* criaNo(int chave) {
    No *no = (No*) malloc(sizeof(No));
    no->chave = chave;
    no->esq = NULL;
    no->dir = NULL;
    no->altura = 0;
    return no;
}

/* Insercao: como em uma ABB, rebalanceando na volta da recursao */
No* inserir(No *raiz, int chave) {
    if (raiz == NULL) return criaNo(chave);
    if (chave < raiz->chave) raiz->esq = inserir(raiz->esq, chave);
    else if (chave > raiz->chave) raiz->dir = inserir(raiz->dir, chave);
    else return raiz;                                     /* repetida: ignora */
    return balancear(raiz);
}

/* f) Remocao: como em uma ABB (predecessor no caso de dois filhos), rebalanceando na volta */
No* remover(No *raiz, int chave) {
    if (raiz == NULL) return NULL;

    if (chave < raiz->chave) {
        raiz->esq = remover(raiz->esq, chave);
    } else if (chave > raiz->chave) {
        raiz->dir = remover(raiz->dir, chave);
    } else if (raiz->esq == NULL || raiz->dir == NULL) {  /* folha ou um filho */
        No *filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
        free(raiz);
        return filho;
    } else {                                              /* dois filhos */
        No *pred = raiz->esq;
        while (pred->dir != NULL) pred = pred->dir;       /* maior da subarvore esquerda */
        raiz->chave = pred->chave;
        raiz->esq = remover(raiz->esq, pred->chave);
    }
    return balancear(raiz);
}

/* e) Percursos */
void preOrdem(No *raiz) {
    if (raiz != NULL) { printf("%d ", raiz->chave); preOrdem(raiz->esq); preOrdem(raiz->dir); }
}
void emOrdem(No *raiz) {
    if (raiz != NULL) { emOrdem(raiz->esq); printf("%d ", raiz->chave); emOrdem(raiz->dir); }
}
void posOrdem(No *raiz) {
    if (raiz != NULL) { posOrdem(raiz->esq); posOrdem(raiz->dir); printf("%d ", raiz->chave); }
}

/* Altura e fator de balanceamento de cada no, em ordem crescente de chave */
static void listaFB(No *raiz) {
    if (raiz != NULL) {
        listaFB(raiz->esq);
        printf("   no %3d : altura = %d | FB = %+d\n", raiz->chave, raiz->altura, fatorBalanceamento(raiz));
        listaFB(raiz->dir);
    }
}

static void liberaArvore(No *raiz) {
    if (raiz != NULL) {
        liberaArvore(raiz->esq);
        liberaArvore(raiz->dir);
        free(raiz);
    }
}

int main() {
    No *raiz = NULL;
    int i;
    int entrada[] = {50, 30, 70, 20, 40, 10, 25, 35, 45, 60, 80, 90, 85};
    int remocoes[] = {30, 70, 50};

    printf("=== INSERCOES ===\n");
    for (i = 0; i < 13; i++) {
        printf("Inserindo %d\n", entrada[i]);
        raiz = inserir(raiz, entrada[i]);
    }

    printf("\n=== ARVORE FINAL ===\n");
    printf("Pre-ordem : "); preOrdem(raiz);  printf("\n");
    printf("Em ordem  : "); emOrdem(raiz);   printf("\n");
    printf("Pos-ordem : "); posOrdem(raiz);  printf("\n");
    printf("Altura da arvore: %d\n", altura(raiz));
    listaFB(raiz);

    printf("\n=== REMOCOES ===\n");
    for (i = 0; i < 3; i++) {
        rotacoes = 0;
        printf("Removendo %d\n", remocoes[i]);
        raiz = remover(raiz, remocoes[i]);
        if (rotacoes == 0) printf("  Rotacao necessaria? NAO\n");
        printf("  Em ordem: "); emOrdem(raiz); printf("\n");
        printf("  Altura da arvore: %d\n", altura(raiz));
        listaFB(raiz);
        printf("\n");
    }
    liberaArvore(raiz);
    return 0;
}

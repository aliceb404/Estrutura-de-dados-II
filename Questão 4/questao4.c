/*
 QUESTAO 4 - Arvore AVL (insercao, remocao e rotacoes LL, RR, LR, RL)
 */

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* a) Estrutura do no da AVL                                           */
/* ------------------------------------------------------------------ */
typedef struct no {
    int chave;
    struct no *esq;
    struct no *dir;
    int altura;
} No;

/* Contador de rotacoes realizadas (para informar se houve rotacao). */
static int g_rotacoes = 0;

/* ------------------------------------------------------------------ */
/* b) Funcoes basicas                                                  */
/* ------------------------------------------------------------------ */
int altura(No *no) {
    return (no == NULL) ? -1 : no->altura;
}

static int maior(int a, int b) {
    return (a > b) ? a : b;
}

static void atualizaAltura(No *no) {
    no->altura = 1 + maior(altura(no->esq), altura(no->dir));
}

int fatorBalanceamento(No *no) {
    return (no == NULL) ? 0 : altura(no->esq) - altura(no->dir);
}

/*
 * Rotacao simples a direita (caso LL):
 *        y                x
 *       / \              / \
 *      x   C    ==>     A   y
 *     / \                  / \
 *    A   B                B   C
 */
No* rotacaoDireita(No *y) {
    No *x = y->esq;
    No *B = x->dir;

    x->dir = y;          /* y passa a ser filho direito de x */
    y->esq = B;          /* B (subarvore do meio) vai para y */

    atualizaAltura(y);   /* y primeiro: agora esta abaixo de x */
    atualizaAltura(x);
    g_rotacoes++;
    return x;            /* nova raiz da subarvore */
}

/*
 * Rotacao simples a esquerda (caso RR):
 *      x                    y
 *     / \                  / \
 *    A   y      ==>       x   C
 *       / \              / \
 *      B   C            A   B
 */
No* rotacaoEsquerda(No *x) {
    No *y = x->dir;
    No *B = y->esq;

    y->esq = x;          /* x passa a ser filho esquerdo de y */
    x->dir = B;          /* B (subarvore do meio) vai para x  */

    atualizaAltura(x);   /* x primeiro: agora esta abaixo de y */
    atualizaAltura(y);
    g_rotacoes++;
    return y;
}

/* ------------------------------------------------------------------ */
/* Balanceamento de um no (usado por inserir E por remover).           */
/* Identifica o caso, imprime o diagnostico e aplica as rotacoes.      */
/* ------------------------------------------------------------------ */
static No* balancear(No *no) {
    int fb;

    atualizaAltura(no);
    fb = fatorBalanceamento(no);

    if (fb > 1) {                                   /* pesada a ESQUERDA */
        int fbEsq = fatorBalanceamento(no->esq);
        if (fbEsq >= 0) {
            printf("   >> Desbalanceamento no no %d (FB = %+d): caso LL "
                   "(FB do filho esquerdo %d = %+d)\n"
                   "      Rotacao necessaria: simples a DIREITA em %d\n",
                   no->chave, fb, no->esq->chave, fbEsq, no->chave);
            return rotacaoDireita(no);
        } else {
            printf("   >> Desbalanceamento no no %d (FB = %+d): caso LR "
                   "(FB do filho esquerdo %d = %+d)\n"
                   "      Rotacao necessaria: dupla = ESQUERDA em %d, depois DIREITA em %d\n",
                   no->chave, fb, no->esq->chave, fbEsq, no->esq->chave, no->chave);
            no->esq = rotacaoEsquerda(no->esq);
            return rotacaoDireita(no);
        }
    }

    if (fb < -1) {                                  /* pesada a DIREITA */
        int fbDir = fatorBalanceamento(no->dir);
        if (fbDir <= 0) {
            printf("   >> Desbalanceamento no no %d (FB = %+d): caso RR "
                   "(FB do filho direito %d = %+d)\n"
                   "      Rotacao necessaria: simples a ESQUERDA em %d\n",
                   no->chave, fb, no->dir->chave, fbDir, no->chave);
            return rotacaoEsquerda(no);
        } else {
            printf("   >> Desbalanceamento no no %d (FB = %+d): caso RL "
                   "(FB do filho direito %d = %+d)\n"
                   "      Rotacao necessaria: dupla = DIREITA em %d, depois ESQUERDA em %d\n",
                   no->chave, fb, no->dir->chave, fbDir, no->dir->chave, no->chave);
            no->dir = rotacaoDireita(no->dir);
            return rotacaoEsquerda(no);
        }
    }

    return no;                                      /* ja balanceado */
}

static No* criaNo(int chave) {
    No *no = (No*) malloc(sizeof(No));
    if (no == NULL) {
        printf("Erro: memoria insuficiente\n");
        exit(1);
    }
    no->chave = chave;
    no->esq = NULL;
    no->dir = NULL;
    no->altura = 0;            /* folha */
    return no;
}

/* ------------------------------------------------------------------ */
/* b) Insercao: insere como em uma ABB e rebalanceia no caminho de     */
/*    volta da recursao.                                               */
/* ------------------------------------------------------------------ */
No* inserir(No *raiz, int chave) {
    if (raiz == NULL) {
        return criaNo(chave);
    }
    if (chave < raiz->chave) {
        raiz->esq = inserir(raiz->esq, chave);
    } else if (chave > raiz->chave) {
        raiz->dir = inserir(raiz->dir, chave);
    } else {
        return raiz;           /* chave repetida: ignora */
    }
    return balancear(raiz);
}

/* ------------------------------------------------------------------ */
/* f) Remocao: remove como em uma ABB e rebalanceia no caminho de      */
/*    volta da recursao.                                               */
/* ------------------------------------------------------------------ */
#ifdef USAR_SUCESSOR
static No* noMinimo(No *no) {
    while (no->esq != NULL) no = no->esq;
    return no;
}
#else
static No* noMaximo(No *no) {
    while (no->dir != NULL) no = no->dir;
    return no;
}
#endif

No* remover(No *raiz, int chave) {
    if (raiz == NULL) {
        return NULL;                         /* chave nao encontrada */
    }

    if (chave < raiz->chave) {
        raiz->esq = remover(raiz->esq, chave);
    } else if (chave > raiz->chave) {
        raiz->dir = remover(raiz->dir, chave);
    } else {
        /* Encontrou o no a remover */
        if (raiz->esq == NULL || raiz->dir == NULL) {
            /* Folha ou um unico filho */
            No *filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            free(raiz);
            return filho;                    /* NULL se era folha */
        }

        /* Dois filhos: troca pela chave do predecessor (ou sucessor) */
#ifdef USAR_SUCESSOR
        {
            No *suc = noMinimo(raiz->dir);
            raiz->chave = suc->chave;
            raiz->dir = remover(raiz->dir, suc->chave);
        }
#else
        {
            No *pred = noMaximo(raiz->esq);
            raiz->chave = pred->chave;
            raiz->esq = remover(raiz->esq, pred->chave);
        }
#endif
    }
    return balancear(raiz);
}

/* ------------------------------------------------------------------ */
/* e) Percursos                                                        */
/* ------------------------------------------------------------------ */
void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->chave);
        emOrdem(raiz->dir);
    }
}

void posOrdem(No *raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%d ", raiz->chave);
    }
}

/* ------------------- funcoes auxiliares de exibicao --------------- */

/* Desenha a arvore "deitada" (raiz a esquerda; direita em cima) com FB. */
static void desenha(No *raiz, int nivel) {
    int i;
    if (raiz == NULL) return;
    desenha(raiz->dir, nivel + 1);
    for (i = 0; i < nivel; i++) printf("         ");
    printf("%d (FB=%+d)\n", raiz->chave, fatorBalanceamento(raiz));
    desenha(raiz->esq, nivel + 1);
}

/* Notacao parentetica: (raiz esquerda direita), '-' = vazio. */
static void parenteses(No *raiz) {
    if (raiz == NULL) { printf("-"); return; }
    if (raiz->esq == NULL && raiz->dir == NULL) { printf("%d", raiz->chave); return; }
    printf("(%d ", raiz->chave);
    parenteses(raiz->esq);
    printf(" ");
    parenteses(raiz->dir);
    printf(")");
}

/* Lista altura e FB de cada no, em ordem crescente de chave. */
static void listaFB(No *raiz) {
    if (raiz != NULL) {
        listaFB(raiz->esq);
        printf("   no %3d : altura = %d | FB = %+d\n",
               raiz->chave, raiz->altura, fatorBalanceamento(raiz));
        listaFB(raiz->dir);
    }
}

/* Confere se TODOS os nos tem |FB| <= 1 e altura coerente. */
static int ehAVL(No *raiz) {
    if (raiz == NULL) return 1;
    {
        int fb = fatorBalanceamento(raiz);
        if (fb < -1 || fb > 1) return 0;
        if (raiz->altura != 1 + maior(altura(raiz->esq), altura(raiz->dir))) return 0;
    }
    return ehAVL(raiz->esq) && ehAVL(raiz->dir);
}

static void mostraArvore(No *raiz, const char *titulo) {
    printf("%s\n", titulo);
    desenha(raiz, 0);
    printf("   Notacao: ");
    parenteses(raiz);
    printf("\n");
}

static void liberaArvore(No *raiz) {
    if (raiz != NULL) {
        liberaArvore(raiz->esq);
        liberaArvore(raiz->dir);
        free(raiz);
    }
}

/* ------------------------------------------------------------------ */
/* Programa principal                                                  */
/* ------------------------------------------------------------------ */
int main() {
    No *raiz = NULL;
    int i;

    int entrada[] = {50, 30, 70, 20, 40, 10, 25, 35, 45, 60, 80, 90, 85};
    int n = sizeof(entrada) / sizeof(entrada[0]);
    int remocoes[] = {30, 70, 50};
    int m = sizeof(remocoes) / sizeof(remocoes[0]);

    /* ---- c) e d) Insercoes, com diagnostico de cada rotacao ---- */
    printf("=========== INSERCOES ===========\n");
    for (i = 0; i < n; i++) {
        g_rotacoes = 0;
        printf("\nInserindo %d\n", entrada[i]);
        raiz = inserir(raiz, entrada[i]);
        if (g_rotacoes > 0) {
            mostraArvore(raiz, "   Arvore apos a rotacao:");
        } else {
            printf("   (sem desbalanceamento)\n");
        }
    }

    /* ---- e) Resultados finais da construcao ---- */
    printf("\n=========== ARVORE FINAL (apos as insercoes) ===========\n");
    mostraArvore(raiz, "");
    printf("Pre-ordem : "); preOrdem(raiz);  printf("\n");
    printf("Em ordem  : "); emOrdem(raiz);   printf("\n");
    printf("Pos-ordem : "); posOrdem(raiz);  printf("\n");
    printf("Altura da arvore: %d\n", altura(raiz));
    printf("Fator de balanceamento de cada no:\n");
    listaFB(raiz);
    printf("Todos os nos com |FB| <= 1 ? %s\n", ehAVL(raiz) ? "SIM" : "NAO");

    /* ---- f) Remocoes ---- */
    printf("\n=========== REMOCOES ===========\n");
    for (i = 0; i < m; i++) {
        g_rotacoes = 0;
        printf("\nRemovendo %d\n", remocoes[i]);
        raiz = remover(raiz, remocoes[i]);
        if (g_rotacoes == 0) {
            printf("   Rotacao necessaria? NAO\n");
        }
        mostraArvore(raiz, "   Arvore resultante:");
        printf("   Em ordem: "); emOrdem(raiz); printf("\n");
        printf("   Altura da arvore: %d\n", altura(raiz));
        printf("   Fatores de balanceamento:\n");
        listaFB(raiz);
        printf("   Todos os nos com |FB| <= 1 ? %s\n", ehAVL(raiz) ? "SIM" : "NAO");
    }

    liberaArvore(raiz);
    return 0;
}
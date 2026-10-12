/*
 * ESTRUTURAS DE DADOS - UFPA / ICEN / Faculdade de Computacao
 * QUESTAO 3 - Arvore Binaria de Busca (ABB)
 *
 * Operacoes implementadas:
 *   1) abb_insere        2) abb_busca (recursiva)   3) abb_remove (3 casos,
 *   predecessor)         4) abb_altura (raiz sozinha = 0)
 *   5) abb_folhas        6) abb_kesimo_menor (sem vetor/estrutura auxiliar)
 *   7) eh_abb            8) percurso em ordem
 *
 * Compilar: gcc -Wall -Wextra -o questao3 questao3.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct arv {
    int info;
    struct arv *esq;
    struct arv *dir;
} Arv;

/* Quando 1, abb_remove explica o que esta fazendo (usado no item f) */
static int abb_trace = 0;

/* ------------------------------------------------------------------ */
/* Cria um no isolado                                                  */
/* ------------------------------------------------------------------ */
Arv* abb_cria_no(int valor) {
    Arv *no = (Arv*) malloc(sizeof(Arv));
    if (no == NULL) {
        printf("Erro: memoria insuficiente\n");
        exit(1);
    }
    no->info = valor;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

/* ------------------------------------------------------------------ */
/* 1. Insercao: menores a esquerda, maiores a direita.                 */
/*    Retorna a (possivelmente nova) raiz. Valores repetidos ignorados. */
/* ------------------------------------------------------------------ */
Arv* abb_insere(Arv *raiz, int valor) {
    if (raiz == NULL) {
        return abb_cria_no(valor);
    }
    if (valor < raiz->info) {
        raiz->esq = abb_insere(raiz->esq, valor);
    } else if (valor > raiz->info) {
        raiz->dir = abb_insere(raiz->dir, valor);
    }
    /* valor == raiz->info: ja existe, nada a fazer */
    return raiz;
}

/* ------------------------------------------------------------------ */
/* 2. Busca recursiva: retorna o endereco do no ou NULL.               */
/* ------------------------------------------------------------------ */
Arv* abb_busca(Arv *raiz, int valor) {
    if (raiz == NULL || raiz->info == valor) {
        return raiz;                              /* achou ou nao existe */
    }
    if (valor < raiz->info) {
        return abb_busca(raiz->esq, valor);
    }
    return abb_busca(raiz->dir, valor);
}

/* Maior elemento de uma subarvore (no mais a direita) - predecessor. */
Arv* abb_maximo(Arv *raiz) {
    while (raiz != NULL && raiz->dir != NULL) {
        raiz = raiz->dir;
    }
    return raiz;
}

/* ------------------------------------------------------------------ */
/* 3. Remocao: trata os tres casos.                                    */
/*    Dois filhos -> substitui pelo PREDECESSOR (maior da subarvore    */
/*    esquerda) e remove o predecessor de la.                          */
/* ------------------------------------------------------------------ */
Arv* abb_remove(Arv *raiz, int valor) {
    if (raiz == NULL) {
        if (abb_trace) printf("   [remove] valor %d nao encontrado\n", valor);
        return NULL;
    }

    if (valor < raiz->info) {
        if (abb_trace) printf("   [remove] %d < %d: desce para a ESQUERDA\n", valor, raiz->info);
        raiz->esq = abb_remove(raiz->esq, valor);
    } else if (valor > raiz->info) {
        if (abb_trace) printf("   [remove] %d > %d: desce para a DIREITA\n", valor, raiz->info);
        raiz->dir = abb_remove(raiz->dir, valor);
    } else {
        /* Encontrou o no a remover */

        /* Caso 1: no folha */
        if (raiz->esq == NULL && raiz->dir == NULL) {
            if (abb_trace) printf("   [remove] no %d e FOLHA: e liberado\n", raiz->info);
            free(raiz);
            return NULL;
        }

        /* Caso 2: apenas um filho -> o filho ocupa o lugar do no */
        if (raiz->esq == NULL || raiz->dir == NULL) {
            Arv *filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            if (abb_trace) printf("   [remove] no %d tem UM filho (%d): o filho sobe\n",
                                  raiz->info, filho->info);
            free(raiz);
            return filho;
        }

        /* Caso 3: dois filhos -> usa o predecessor */
        {
            Arv *pred = abb_maximo(raiz->esq);
            if (abb_trace) printf("   [remove] no %d tem DOIS filhos: predecessor = %d "
                                  "(maior da subarvore esquerda)\n", raiz->info, pred->info);
            raiz->info = pred->info;                        /* copia a chave   */
            if (abb_trace) printf("   [remove] chave %d copiada para o no; agora remove %d "
                                  "da subarvore esquerda\n", raiz->info, pred->info);
            raiz->esq = abb_remove(raiz->esq, pred->info);  /* remove original */
        }
    }
    return raiz;
}

/* ------------------------------------------------------------------ */
/* 4. Altura: arvore vazia = -1; arvore so com a raiz = 0.             */
/* ------------------------------------------------------------------ */
int abb_altura(Arv *raiz) {
    int he, hd;
    if (raiz == NULL) {
        return -1;
    }
    he = abb_altura(raiz->esq);
    hd = abb_altura(raiz->dir);
    return 1 + (he > hd ? he : hd);
}

/* ------------------------------------------------------------------ */
/* 5. Quantidade de folhas (recursiva)                                 */
/* ------------------------------------------------------------------ */
int abb_folhas(Arv *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    if (raiz->esq == NULL && raiz->dir == NULL) {
        return 1;
    }
    return abb_folhas(raiz->esq) + abb_folhas(raiz->dir);
}

/* ------------------------------------------------------------------ */
/* 6. K-esimo menor elemento                                           */
/*    Percurso em ordem que apenas CONTA os nos visitados: nenhum      */
/*    vetor ou outra estrutura guarda os elementos. 'cont' e so um     */
/*    contador de quantos nos ja foram visitados.                      */
/* ------------------------------------------------------------------ */
static int kesimo_aux(Arv *raiz, int k, int *cont, int *valor) {
    if (raiz == NULL) {
        return 0;
    }
    if (kesimo_aux(raiz->esq, k, cont, valor)) {   /* ja achou na esquerda */
        return 1;
    }
    (*cont)++;                                     /* visita o no atual    */
    if (*cont == k) {
        *valor = raiz->info;
        return 1;
    }
    return kesimo_aux(raiz->dir, k, cont, valor);
}

/* Retorna 1 e grava em *valor se existir o k-esimo menor; 0 caso contrario. */
int abb_kesimo_menor(Arv *raiz, int k, int *valor) {
    int cont = 0;
    if (k <= 0 || valor == NULL) {
        return 0;
    }
    return kesimo_aux(raiz, k, &cont, valor);
}

/* ------------------------------------------------------------------ */
/* 7. Verificacao de ABB                                               */
/*    Cada no precisa estar dentro de um intervalo (min, max) herdado  */
/*    dos ancestrais - nao basta comparar com os filhos imediatos.     */
/* ------------------------------------------------------------------ */
static int eh_abb_aux(Arv *raiz, long long min, long long max) {
    if (raiz == NULL) {
        return 1;
    }
    if (raiz->info <= min || raiz->info >= max) {   /* fora do intervalo */
        return 0;
    }
    return eh_abb_aux(raiz->esq, min, raiz->info) &&   /* esq: < info */
           eh_abb_aux(raiz->dir, raiz->info, max);     /* dir: > info */
}

int eh_abb(Arv *raiz) {
    return eh_abb_aux(raiz, LLONG_MIN, LLONG_MAX);
}

/* ------------------------------------------------------------------ */
/* 8. Percurso em ordem (esquerda, raiz, direita) -> ordem crescente   */
/* ------------------------------------------------------------------ */
void abb_em_ordem(Arv *raiz) {
    if (raiz != NULL) {
        abb_em_ordem(raiz->esq);
        printf("%d ", raiz->info);
        abb_em_ordem(raiz->dir);
    }
}

/* ------------------- funcoes auxiliares de exibicao --------------- */

/* Desenha a arvore "deitada": a raiz fica a esquerda, a subarvore
 * direita em cima e a esquerda embaixo. */
void abb_desenha(Arv *raiz, int nivel) {
    int i;
    if (raiz == NULL) return;
    abb_desenha(raiz->dir, nivel + 1);
    for (i = 0; i < nivel; i++) printf("      ");
    printf("%d\n", raiz->info);
    abb_desenha(raiz->esq, nivel + 1);
}

/* Notacao parentetica: (raiz esquerda direita), '-' = vazio. */
void abb_parenteses(Arv *raiz) {
    if (raiz == NULL) {
        printf("-");
        return;
    }
    if (raiz->esq == NULL && raiz->dir == NULL) {
        printf("%d", raiz->info);
        return;
    }
    printf("(%d ", raiz->info);
    abb_parenteses(raiz->esq);
    printf(" ");
    abb_parenteses(raiz->dir);
    printf(")");
}

void abb_mostra(Arv *raiz, const char *titulo) {
    printf("\n%s\n", titulo);
    abb_desenha(raiz, 0);
    printf("Notacao: ");
    abb_parenteses(raiz);
    printf("\nEm ordem: ");
    abb_em_ordem(raiz);
    printf("\n");
}

void abb_libera(Arv *raiz) {
    if (raiz != NULL) {
        abb_libera(raiz->esq);
        abb_libera(raiz->dir);
        free(raiz);
    }
}

/* ------------------------------------------------------------------ */
/* Programa principal                                                  */
/* ------------------------------------------------------------------ */
int main() {
    Arv *raiz = NULL;
    int i, valor;

    /* ---- Arvore inicial: insercoes sucessivas ---- */
    int inicial[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 65};
    int n = sizeof(inicial) / sizeof(inicial[0]);
    for (i = 0; i < n; i++) {
        raiz = abb_insere(raiz, inicial[i]);
    }
    abb_mostra(raiz, "=== Arvore inicial ===");
    printf("eh_abb = %d\n", eh_abb(raiz));

    /* ---- Sequencia de operacoes ---- */
    printf("\n>>> 1. Inserir 35");
    raiz = abb_insere(raiz, 35);

    printf("\n>>> 2. Inserir 75");
    raiz = abb_insere(raiz, 75);
    abb_mostra(raiz, "(arvore apos as insercoes 35 e 75)");

    abb_trace = 1;   /* mostra o passo a passo das remocoes (item f) */

    printf("\n>>> 3. Remover 20\n");
    raiz = abb_remove(raiz, 20);
    abb_mostra(raiz, "--- Arvore apos remover 20 ---");

    printf("\n>>> 4. Remover 70\n");
    raiz = abb_remove(raiz, 70);
    abb_mostra(raiz, "--- Arvore apos remover 70 ---");

    printf("\n>>> 5. Remover 50\n");
    raiz = abb_remove(raiz, 50);
    abb_mostra(raiz, "--- Arvore apos remover 50 ---");

    abb_trace = 0;

    printf("\n>>> 6. Inserir 55");
    raiz = abb_insere(raiz, 55);

    printf("\n>>> 7. Inserir 15");
    raiz = abb_insere(raiz, 15);

    abb_mostra(raiz, "=== Arvore final ===");

    /* ---- Tarefas adicionais ---- */
    printf("\n=== Resultados ===\n");
    printf("b) Percurso em ordem : ");
    abb_em_ordem(raiz);
    printf("\n");
    printf("c) Altura da arvore  : %d\n", abb_altura(raiz));
    printf("d) Numero de folhas  : %d\n", abb_folhas(raiz));
    if (abb_kesimo_menor(raiz, 5, &valor)) {
        printf("e) 5o menor elemento : %d\n", valor);
    }

    /* ---- Testes complementares ---- */
    printf("\n=== Testes complementares ===\n");
    printf("k-esimo menor para todo k: ");
    for (i = 1; abb_kesimo_menor(raiz, i, &valor); i++) {
        printf("k=%d->%d  ", i, valor);
    }
    printf("\nk=%d (alem do tamanho) retorna %d\n", i, abb_kesimo_menor(raiz, i, &valor));

    printf("abb_busca(35) = %s | abb_busca(99) = %s\n",
           abb_busca(raiz, 35) ? "encontrado" : "NULL",
           abb_busca(raiz, 99) ? "encontrado" : "NULL");
    printf("eh_abb(arvore final) = %d\n", eh_abb(raiz));

    /* Arvore INVALIDA: cada no respeita seus filhos imediatos, mas o 5 esta
     * na subarvore direita do 10 -> deve retornar 0:
     *        10
     *          \
     *           20
     *          /
     *         5                                                       */
    {
        Arv *ruim = abb_cria_no(10);
        ruim->dir = abb_cria_no(20);
        ruim->dir->esq = abb_cria_no(5);
        printf("eh_abb(arvore invalida 10-20-5) = %d\n", eh_abb(ruim));
        abb_libera(ruim);
    }

    abb_libera(raiz);
    return 0;
}
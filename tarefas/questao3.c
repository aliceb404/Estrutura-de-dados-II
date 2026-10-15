/* QUESTAO 3 - Arvore Binaria de Busca (ABB) */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct arv {
    int info;
    struct arv *esq;
    struct arv *dir;
} Arv;

Arv* abb_cria_no(int valor) {
    Arv *no = (Arv*) malloc(sizeof(Arv));
    no->info = valor;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

/* 1. Insercao: menores a esquerda, maiores a direita (repetidos sao ignorados) */
Arv* abb_insere(Arv *raiz, int valor) {
    if (raiz == NULL) return abb_cria_no(valor);
    if (valor < raiz->info) raiz->esq = abb_insere(raiz->esq, valor);
    else if (valor > raiz->info) raiz->dir = abb_insere(raiz->dir, valor);
    return raiz;
}

/* 2. Busca recursiva: retorna o no encontrado ou NULL */
Arv* abb_busca(Arv *raiz, int valor) {
    if (raiz == NULL || raiz->info == valor) return raiz;
    if (valor < raiz->info) return abb_busca(raiz->esq, valor);
    return abb_busca(raiz->dir, valor);
}

/* Maior elemento de uma subarvore (no mais a direita) = predecessor */
Arv* abb_maximo(Arv *raiz) {
    while (raiz->dir != NULL) raiz = raiz->dir;
    return raiz;
}

/* 3. Remocao: tres casos; com dois filhos usa o predecessor */
Arv* abb_remove(Arv *raiz, int valor) {
    if (raiz == NULL) return NULL;

    if (valor < raiz->info) {
        raiz->esq = abb_remove(raiz->esq, valor);
    } else if (valor > raiz->info) {
        raiz->dir = abb_remove(raiz->dir, valor);
    } else {
        if (raiz->esq == NULL && raiz->dir == NULL) {          /* caso 1: folha */
            free(raiz);
            return NULL;
        }
        if (raiz->esq == NULL || raiz->dir == NULL) {          /* caso 2: um filho */
            Arv *filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            free(raiz);
            return filho;
        }
        {                                                      /* caso 3: dois filhos */
            Arv *pred = abb_maximo(raiz->esq);
            raiz->info = pred->info;                           /* copia a chave do predecessor */
            raiz->esq = abb_remove(raiz->esq, pred->info);     /* remove o original (folha ou 1 filho) */
        }
    }
    return raiz;
}

/* 4. Altura: raiz sozinha = 0 (arvore vazia = -1) */
int abb_altura(Arv *raiz) {
    int he, hd;
    if (raiz == NULL) return -1;
    he = abb_altura(raiz->esq);
    hd = abb_altura(raiz->dir);
    return 1 + (he > hd ? he : hd);
}

/* 5. Quantidade de folhas */
int abb_folhas(Arv *raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esq == NULL && raiz->dir == NULL) return 1;
    return abb_folhas(raiz->esq) + abb_folhas(raiz->dir);
}

/* 6. k-esimo menor: percurso em ordem que so conta os nos visitados (sem vetor) */
static int kesimo_aux(Arv *raiz, int k, int *cont, int *valor) {
    if (raiz == NULL) return 0;
    if (kesimo_aux(raiz->esq, k, cont, valor)) return 1;
    (*cont)++;
    if (*cont == k) {
        *valor = raiz->info;
        return 1;
    }
    return kesimo_aux(raiz->dir, k, cont, valor);
}

int abb_kesimo_menor(Arv *raiz, int k, int *valor) {
    int cont = 0;
    return kesimo_aux(raiz, k, &cont, valor);
}

/* 7. Verificacao de ABB: cada no deve estar dentro do intervalo (min, max)
 *    herdado dos ancestrais, e nao apenas ser coerente com os filhos */
static int eh_abb_aux(Arv *raiz, long long min, long long max) {
    if (raiz == NULL) return 1;
    if (raiz->info <= min || raiz->info >= max) return 0;
    return eh_abb_aux(raiz->esq, min, raiz->info) && eh_abb_aux(raiz->dir, raiz->info, max);
}

int eh_abb(Arv *raiz) {
    return eh_abb_aux(raiz, LLONG_MIN, LLONG_MAX);
}

/* 8. Percurso em ordem (ordem crescente) */
void abb_em_ordem(Arv *raiz) {
    if (raiz != NULL) {
        abb_em_ordem(raiz->esq);
        printf("%d ", raiz->info);
        abb_em_ordem(raiz->dir);
    }
}

void abb_libera(Arv *raiz) {
    if (raiz != NULL) {
        abb_libera(raiz->esq);
        abb_libera(raiz->dir);
        free(raiz);
    }
}

static void mostra(const char *titulo, Arv *raiz) {
    printf("%-28s: ", titulo);
    abb_em_ordem(raiz);
    printf("\n");
}

int main() {
    Arv *raiz = NULL;
    int i, valor;
    int inicial[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 65};

    for (i = 0; i < 10; i++) raiz = abb_insere(raiz, inicial[i]);
    mostra("Arvore inicial", raiz);

    raiz = abb_insere(raiz, 35);                       /* 1 */
    raiz = abb_insere(raiz, 75);                       /* 2 */
    mostra("Apos inserir 35 e 75", raiz);
    raiz = abb_remove(raiz, 20);                       /* 3 */
    mostra("Apos remover 20", raiz);
    raiz = abb_remove(raiz, 70);                       /* 4 */
    mostra("Apos remover 70", raiz);
    raiz = abb_remove(raiz, 50);                       /* 5 */
    mostra("Apos remover 50", raiz);
    raiz = abb_insere(raiz, 55);                       /* 6 */
    raiz = abb_insere(raiz, 15);                       /* 7 */

    printf("\nb) Percurso em ordem final : ");
    abb_em_ordem(raiz);
    printf("\nc) Altura da arvore        : %d", abb_altura(raiz));
    printf("\nd) Numero de folhas        : %d", abb_folhas(raiz));
    if (abb_kesimo_menor(raiz, 5, &valor))
        printf("\ne) 5o menor elemento       : %d", valor);
    printf("\neh_abb(arvore final)       : %d", eh_abb(raiz));
    printf("\nabb_busca(35)              : %s\n", abb_busca(raiz, 35) ? "encontrado" : "NULL");
    abb_libera(raiz);
    return 0;
}

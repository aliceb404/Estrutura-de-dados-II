/*
 * ESTRUTURAS DE DADOS - UFPA / ICEN / Faculdade de Computacao
 * QUESTAO 2 - Permutacoes de 1,2,3,4 obtidas como saida de uma pilha
 *
 * Os inteiros 1,2,3,4 chegam NESTA ORDEM. A qualquer momento podemos:
 *      PUSH  -> empilhar o proximo inteiro da entrada
 *      POP   -> desempilhar o topo (o elemento vai para a saida)
 * O programa gera as 24 permutacoes e, para cada uma, tenta construi-la
 * simulando a pilha. Se conseguir, mostra a sequencia de operacoes;
 * se nao, mostra o motivo.
 *
 * Estrategia de simulacao (e correta, pois nao ha escolha alternativa):
 *   Para cada valor v que deve SAIR:
 *     1) se v ainda nao entrou, empilha todos os inteiros da entrada
 *        ate empilhar v;
 *     2) o topo precisa ser v. Se nao for, v esta ENTERRADO sob outro
 *        elemento que ainda precisa sair antes (violacao do LIFO).
 *
 * Compilar: gcc -Wall -Wextra -o questao2 questao2.c
 */

#include <stdio.h>
#include <string.h>

#define N 4

/* Simula a pilha para obter a permutacao perm[0..n-1].
 * Retorna 1 se possivel (ops recebe a sequencia de operacoes);
 * retorna 0 se impossivel (alvo/topo explicam a falha). */
int simula(int perm[], int n, char *ops, int *falha_alvo, int *falha_topo) {
    int pilha[N];
    int topo = -1;
    int proximo = 1;            /* proximo inteiro da entrada */
    int i;
    char tmp[32];

    ops[0] = '\0';

    for (i = 0; i < n; i++) {
        int alvo = perm[i];

        /* Empilha ate que 'alvo' tenha entrado na pilha */
        while (proximo <= alvo) {
            pilha[++topo] = proximo;
            sprintf(tmp, "Empilha %d; ", proximo);
            strcat(ops, tmp);
            proximo++;
        }

        /* O topo precisa ser exatamente o alvo */
        if (pilha[topo] == alvo) {
            topo--;
            sprintf(tmp, "Desempilha %d; ", alvo);
            strcat(ops, tmp);
        } else {
            *falha_alvo = alvo;
            *falha_topo = pilha[topo];
            return 0;
        }
    }
    return 1;
}

/* Processa uma permutacao: simula e imprime o resultado. */
static int total = 0, possiveis = 0;

void processa(int perm[]) {
    char ops[256];
    int alvo = 0, topo = 0;
    int ok = simula(perm, N, ops, &alvo, &topo);

    total++;
    printf("%2d) %d,%d,%d,%d  -> ", total, perm[0], perm[1], perm[2], perm[3]);
    if (ok) {
        possiveis++;
        printf("POSSIVEL\n      %s\n", ops);
    } else {
        printf("IMPOSSIVEL\n      Para sair %d, o topo e %d (que ainda precisa "
               "sair depois): %d esta preso abaixo de %d\n",
               alvo, topo, alvo, topo);
    }
}

/* Gera todas as permutacoes por backtracking (ordem lexicografica). */
void gera(int perm[], int usado[], int pos) {
    int v;
    if (pos == N) {
        processa(perm);
        return;
    }
    for (v = 1; v <= N; v++) {
        if (!usado[v]) {
            usado[v] = 1;
            perm[pos] = v;
            gera(perm, usado, pos + 1);
            usado[v] = 0;
        }
    }
}

int main() {
    int perm[N];
    int usado[N + 1] = {0};

    printf("Permutacoes de 1,2,3,4 como saida de uma pilha\n");
    printf("==============================================\n");
    gera(perm, usado, 0);

    printf("\nResumo: %d possiveis e %d impossiveis (de %d permutacoes)\n",
           possiveis, total - possiveis, total);
    printf("(14 = numero de Catalan C4 = (1/5) * C(8,4))\n");
    return 0;
}
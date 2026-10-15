/* QUESTAO 2 - Permutacoes de 1,2,3,4 obtidas como saida de uma pilha */
#include <stdio.h>
#include <string.h>

#define N 4

/* Simula a pilha para obter a permutacao perm[].
 * Retorna 1 se possivel (ops recebe as operacoes: E = Empilha, D = Desempilha)
 * e 0 se impossivel. */
int simula(int perm[], char *ops) {
    int pilha[N], topo = -1, proximo = 1, i;
    char tmp[16];

    ops[0] = '\0';
    for (i = 0; i < N; i++) {
        while (proximo <= perm[i]) {               /* empilha ate perm[i] entrar */
            pilha[++topo] = proximo;
            sprintf(tmp, "E%d ", proximo++);
            strcat(ops, tmp);
        }
        if (topo < 0 || pilha[topo] != perm[i]) {  /* perm[i] ficou preso embaixo */
            return 0;
        }
        topo--;
        sprintf(tmp, "D%d ", perm[i]);
        strcat(ops, tmp);
    }
    return 1;
}

int main() {
    int a, b, c, d, total = 0, possiveis = 0;
    char ops[64];

    for (a = 1; a <= N; a++)
    for (b = 1; b <= N; b++)
    for (c = 1; c <= N; c++)
    for (d = 1; d <= N; d++) {
        int perm[N] = {a, b, c, d};
        if (a == b || a == c || a == d || b == c || b == d || c == d) continue;

        total++;
        if (simula(perm, ops)) {
            possiveis++;
            printf("%2d) %d %d %d %d  POSSIVEL    %s\n", total, a, b, c, d, ops);
        } else {
            printf("%2d) %d %d %d %d  IMPOSSIVEL\n", total, a, b, c, d);
        }
    }
    printf("\n%d possiveis e %d impossiveis (de %d)\n", possiveis, total - possiveis, total);
    return 0;
}

/* QUESTAO 1 - Pilha sequencial (LIFO) de 10 espacos para paginas web visitadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

typedef struct {
    char adress[30];
    int connectionType;
} PageData;

/* a) Struct da pilha: array correspondente a LIFO + int para guardar o topo */
typedef struct {
    PageData dados[MAX];
    int topo;                      /* -1 indica pilha vazia */
} Stack;

/* Codigos de retorno */
#define OK                0
#define ERRO_PILHA_CHEIA -1        /* overflow  */
#define ERRO_PILHA_VAZIA -2        /* underflow */

/* b) Aloca a pilha */
Stack* inicializarPilha() {
    Stack* pilha = (Stack*) malloc(sizeof(Stack));
    if (pilha != NULL) {
        pilha->topo = -1;
    }
    return pilha;
}

/* c) Adiciona um elemento ao topo */
int push(Stack* pilha, char* adress, int connectionType) {
    if (pilha->topo == MAX - 1) {                        /* pilha cheia */
        printf("PUSH falhou: pilha cheia\n");
        return ERRO_PILHA_CHEIA;
    }
    pilha->topo++;
    strcpy(pilha->dados[pilha->topo].adress, adress);
    pilha->dados[pilha->topo].connectionType = connectionType;
    printf("PUSH: %-26s (conexao tipo %d) | topo = %d\n", adress, connectionType, pilha->topo);
    return OK;
}

/* d) Remove e mostra o ultimo endereco da pilha */
int pop(Stack* pilha) {
    PageData removido;
    if (pilha->topo == -1) {                             /* pilha vazia */
        printf("POP falhou: pilha vazia\n");
        return ERRO_PILHA_VAZIA;
    }
    removido = pilha->dados[pilha->topo];
    pilha->topo--;
    printf("POP : %-26s (conexao tipo %d) | topo = %d\n",
           removido.adress, removido.connectionType, pilha->topo);
    return OK;
}

int main() {
    int errorCode;
    Stack* pilha;

    pilha = inicializarPilha();   // Pilha criada
    errorCode = push(pilha, "http://icen.ufpa.br", 2);
    errorCode = push(pilha, "http://www.youtube.com", 2);
    errorCode = push(pilha, "https://www.facebook.com", 1);
    errorCode = pop(pilha);
    errorCode = push(pilha, "https://drive.google.com", 1);
    errorCode = pop(pilha);
    errorCode = pop(pilha);
    errorCode = pop(pilha);

    (void) errorCode;
    free(pilha);
    return 0;
}

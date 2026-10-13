/*
QUESTAO 1 - Pilha sequencial (LIFO) de 10 espacos para paginas web visitadas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

typedef struct {
    char adress[30];
    int connectionType;
} PageData;

/* ------------------------------------------------------------------ */
/* a) Crie uma struct para abrigar a pilha e defina-a como tipo Stack. Essa struct deve
conter o array correspondente à LIFO e um int para guardar o topo da pilha.
/* ------------------------------------------------------------------ */
typedef struct {
    PageData dados[MAX];
    int topo;
} Stack;

/* Codigos de retorno das operacoes */
#define OK                    0
#define ERRO_PILHA_CHEIA     -1   /* overflow  */
#define ERRO_PILHA_VAZIA     -2   /* underflow */
#define ERRO_PILHA_NULA      -3   /* ponteiro invalido / sem memoria */
#define ERRO_ENDERECO_GRANDE -4   /* string nao cabe em adress[30]   */

/* ------------------------------------------------------------------ */
/* b) Escreva a função inicializarPilha que aloca memória para a pilha.*/
/* ------------------------------------------------------------------ */
Stack* inicializarPilha() {
    Stack* pilha = (Stack*) malloc(sizeof(Stack));
    if (pilha != NULL) {
        pilha->topo = -1;           /* -1 indica pilha vazia */
    }
    return pilha;
}

/* ------------------------------------------------------------------ */
/* c) Escreva a função push para adicionar um elemento à pilha. Se necessário, utilize a
função strcpy(char* destino, const char* origem), da biblioteca padrão, para copiar
strings.                                */
/* ------------------------------------------------------------------ */
int push(Stack* pilha, char* adress, int connectionType) {
    if (pilha == NULL || adress == NULL) {
        return ERRO_PILHA_NULA;
    }
    if (pilha->topo == MAX - 1) {                       /* pilha cheia */
        printf("PUSH falhou: pilha cheia (%d/%d) -> \"%s\" nao foi inserido\n",
               MAX, MAX, adress);
        return ERRO_PILHA_CHEIA;
    }
    if (strlen(adress) >= sizeof(pilha->dados[0].adress)) {  /* nao cabe */
        printf("PUSH falhou: endereco com mais de 29 caracteres\n");
        return ERRO_ENDERECO_GRANDE;
    }

    pilha->topo++;                                       /* avanca o topo */
    strcpy(pilha->dados[pilha->topo].adress, adress);    /* copia a string */
    pilha->dados[pilha->topo].connectionType = connectionType;

    printf("PUSH: %-26s (conexao tipo %d) | topo = %d\n",
           adress, connectionType, pilha->topo);
    return OK;
}

/* ------------------------------------------------------------------ */
/* d)Escreva a função pop para remover e mostrar o último endereço da pilha.*/
/*    Retorna OK ou um codigo de erro.                                 */
/* ------------------------------------------------------------------ */

int pop(Stack* pilha) {
    if (pilha == NULL) {
        return ERRO_PILHA_NULA;
    }
    if (pilha->topo == -1) {                             /* pilha vazia */
        printf("POP falhou: pilha vazia\n");
        return ERRO_PILHA_VAZIA;
    }

    PageData removido = pilha->dados[pilha->topo];       /* guarda o topo */
    pilha->topo--;                                       /* desce o topo  */

    printf("POP : %-26s (conexao tipo %d) | topo = %d\n",
           removido.adress, removido.connectionType, pilha->topo);
    return OK;
}

/* ----------------------- funcoes auxiliares ----------------------- */

/* Mostra a pilha do topo para a base (usada apenas no teste extra). */
void mostrarPilha(Stack* pilha) {
    int i;
    printf("Pilha (topo -> base):");
    if (pilha->topo == -1) {
        printf(" [vazia]");
    }
    for (i = pilha->topo; i >= 0; i--) {
        printf(" [%s]", pilha->dados[i].adress);
    }
    printf("\n");
}

/* Informa o codigo de erro quando diferente de OK. */
void tratarErro(int codigo) {
    if (codigo != OK) {
        printf("   -> codigo de erro retornado: %d\n", codigo);
    }
}

/* Libera a memoria alocada para a pilha. */
void liberarPilha(Stack* pilha) {
    free(pilha);
}

/* ------------------------------------------------------------------ */
/* FUNÇÃO MAIN */
/* ------------------------------------------------------------------ */
int main() {
    int errorCode;
    Stack* pilha;

    pilha = inicializarPilha();   // Pilha criada
    if (pilha == NULL) {
        printf("Erro: memoria insuficiente\n");
        return 1;
    }

    errorCode = push(pilha, "http://icen.ufpa.br", 2);        tratarErro(errorCode);
    errorCode = push(pilha, "http://www.youtube.com", 2);     tratarErro(errorCode);
    errorCode = push(pilha, "https://www.facebook.com", 1);   tratarErro(errorCode);
    errorCode = pop(pilha);                                   tratarErro(errorCode);
    errorCode = push(pilha, "https://drive.google.com", 1);   tratarErro(errorCode);
    errorCode = pop(pilha);                                   tratarErro(errorCode);
    errorCode = pop(pilha);                                   tratarErro(errorCode);
    errorCode = pop(pilha);                                   tratarErro(errorCode);

#ifdef TESTE_EXTRA
    /* ---- Teste extra: underflow e overflow ---- */
    printf("\n--- Teste extra ---\n");
    errorCode = pop(pilha);                                   /* pilha vazia */
    tratarErro(errorCode);

    {
        int i;
        char url[30];
        for (i = 1; i <= MAX + 1; i++) {                      /* 11 pushes   */
            sprintf(url, "http://site%d.com", i);
            errorCode = push(pilha, url, i % 3);
            tratarErro(errorCode);
        }
    }
    mostrarPilha(pilha);
#endif

    liberarPilha(pilha);
    return 0;
}
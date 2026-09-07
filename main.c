#include <stdio.h>
#include <stdlib.h>
#include "validacao.h"

int main(int argc, char *argv[]) {
    if (validarArgumentos(argc, argv) != 0){
        fprintf(stderr, "Erro na declaração do comando\n");
        return 1;
    }

    FILE *arquivo = abrirArquivo(argv[2]);
    if (arquivo == NULL){
        return 1;
    }

    long tempoTotal;
    if (lerTempoTotal(arquivo, &tempoTotal) != 0){
        fprintf(stderr, "Tempo total do arquivo invalido\n");
        fclose(arquivo);
        return 1;
    }

    int quantidade;
    int linhaErro;
    Tarefa *tarefas = lerTarefas(arquivo, &quantidade, &linhaErro);
    fclose(arquivo);

    printf("Algoritmo: %s\n", argv[1]);
    printf("Tempo total: %ld\n", tempoTotal);
    printf("Tarefas carregadas: %d\n", quantidade);

    for (int i = 0; i < quantidade; i++) {
        printf("  [%d] nome=%s periodo=%ld deadline=%ld burst=%ld\n",
               i, tarefas[i].nome, tarefas[i].periodo,
               tarefas[i].deadline, tarefas[i].burst);
    }

    free(tarefas);
    return 0;
}
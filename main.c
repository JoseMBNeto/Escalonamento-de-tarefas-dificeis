#include <stdio.h>
#include <stdlib.h>
#include "validacao.h"
#include "tarefa.h"

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
        fclose(arquivo);
        return 1;
    }

    int quantidade = 0;
    int linhaErro = 0;
    Tarefa *tarefas = lerTarefas(arquivo, &quantidade, &linhaErro);
    fclose(arquivo);

    if (tarefas == NULL){
        if (linhaErro == -1){
            fprintf(stderr, "Erro: falhou ao alocar memoria\n");
        }else{
            fprintf(stderr, "Erro: linha %d invalida\n", linhaErro);
        }
        return 1;
    }

    inicializarSimulacao(tarefas, quantidade);

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
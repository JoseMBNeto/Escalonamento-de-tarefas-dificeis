#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validacao.h"
#include "tarefa.h"
#include "execucao.h"

#define LOGIN "jmbn"

void Resultado (const char *algoritmo, Tarefa *tarefas, int quantidade, TrechoExecucao *trechos, int quantidadeTrechos, FILE *saida){
    if (strcmp(algoritmo, "rate") == 0){
        fprintf(saida, "EXECUTION BY RATE\n");
    }else{
        fprintf(saida, "EXECUTION BY EDF\n");
    }

    for (int i = 0; i < quantidadeTrechos; i++){
        if (trechos[i].indiceTarefa == -1){
            fprintf(saida, "idle for %d units\n", trechos[i].duracao);
        }else{
            fprintf(saida, "[%s] for %d units - %c\n", tarefas[trechos[i].indiceTarefa].nome, trechos[i].duracao, trechos[i].letra);
        }
    }

    fprintf(saida, "\nLOST DEADLINES\n");
    for (int i = 0; i < quantidade; i++){
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].perdidas);
    }

    fprintf(saida, "\nCOMLETE EXECUTION\n");
    for (int i = 0; i < quantidade; i++){
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].completadas);
    }

    fprintf(saida, "\nKILLED\n");
    for (int i = 0; i < quantidade; i++){
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].killed);
    }
}

int main(int argc, char *argv[]) {
    if (validarArgumentos(argc, argv) != 0){
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

    TrechoExecucao *trechos = NULL;
    int quantidadeTrechos = 0;
    int capacidadeTrechos = 0;
    int indiceAnterior = -2;
    int duracaoAtual = 0;

    for (long t = 0; t < tempoTotal; t++){
        for (int i = 0; i < quantidade; i++){
            if (precisaNovaInstancia(t, tarefas[i].periodo)){
                novaInstancia(&tarefas[i], t);
            }
        }

        detectarPerdaDeadline(tarefas, quantidade, t);
        int indiceExecutando = escolherProximaTarefa(tarefas, quantidade, argv[1]);
        trechos = processarInstanteLog(trechos, &quantidadeTrechos, &capacidadeTrechos, tarefas, indiceExecutando, &indiceAnterior, &duracaoAtual);
        executarTarefa(tarefas, indiceExecutando);
    }

    trechos = fecharUltimoTrecho(trechos, &quantidadeTrechos, &capacidadeTrechos, indiceAnterior, duracaoAtual, tarefas);
    if (tempoTotal > 0){
        if (trechos == NULL){
            fprintf(stderr, "Erro: falha ao registrar a execucao\n");
            free(tarefas);
            return 1;
        }

        if (quantidadeTrechos == 0){
            fprintf(stderr, "Erro: falha ao registrar a execucao\n");
            free(tarefas);
            return 1;
        }
    }

    for (int i = 0; i < quantidade; i++){
        if (tarefas[i].ativa){
            tarefas[i].killed = 1;
        }
    }

    char nomeArquivo [64];
    sprintf(nomeArquivo, "%s_%s.out", argv[1], LOGIN);

    FILE *saida = fopen(arquivo, "w");
    if (saida == NULL){
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo '%s'\n", nomeArquivo);
        free(trechos);
        free(tarefas);
        return 1;
    }

    Resultado(argv[1], tarefas, quantidade, trechos, quantidadeTrechos, saida);

    fclose(saida);
    free(trechos);
    free(tarefas);
    return 0;
}
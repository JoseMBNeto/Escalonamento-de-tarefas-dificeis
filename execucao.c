#include <stdlib.h>
#include "execucao.h"

char determinarLetra(Tarefa *tarefas, int indiceTarefa){
    if (indiceTarefa == -1){
        return ' ';
    }

    if (tarefas[indiceTarefa].burstRestante == 0){
        return 'F';
    }

    if (tarefas[indiceTarefa].ativa == 0){
        return 'L';
    }

    return 'H';
}

TrechoExecucao *processarInstanteLog(TrechoExecucao *trechos, int *quantidadeTrechos, int *capacidadeTrechos, Tarefa *tarefas, int indiceExecutando, int *indiceAnterior, int *duracaoAtual){
    if (*indiceAnterior == -2){
        *indiceAnterior = indiceExecutando;
        *duracaoAtual = 1;
        return trechos;
    }

    if (indiceExecutando == *indiceAnterior){
        (*duracaoAtual)++;
        return trechos;
    }

    if (*quantidadeTrechos == *capacidadeTrechos){
        int novaCapacidade;
        if (*capacidadeTrechos == 0){
            novaCapacidade = 4;
        }else{
            novaCapacidade = *capacidadeTrechos * 2;
        }

        TrechoExecucao *novo = realloc(trechos, novaCapacidade * sizeof(TrechoExecucao));
        if (novo == NULL){
            return NULL;
        }
        trechos = novo;
        *capacidadeTrechos = novaCapacidade;
    }

    trechos[*quantidadeTrechos].indiceTarefa = *indiceAnterior;
    trechos[*quantidadeTrechos].duracao = *duracaoAtual;
    trechos[*quantidadeTrechos].letra = determinarLetra(tarefas, *indiceAnterior);
    (*quantidadeTrechos)++;
    *indiceAnterior = indiceExecutando;
    *duracaoAtual = 1;
    return trechos;
}

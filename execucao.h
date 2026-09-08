#ifndef EXECUCAO_H
#define EXECUCAO_H

#include "tarefa.h"

typedef struct {
    int indiceTarefa;
    int duracao;
    char letra;
} TrechoExecucao;

TrechoExecucao *processarInstanteLog(TrechoExecucao *trechos, int *quantidadeTrechos, int *capacidadeTrechos, Tarefa *tarefas, int indiceExecutando, int *indiceAnterior, int *duracaoAtual);
TrechoExecucao *fecharUltimoTrecho(TrechoExecucao *trechos, int *quantidadeTrechos, int *capacidadeTrechos, int indiceAnterior, int duracaoAtual, Tarefa *tarefas);
char determinarLetra(Tarefa *tarefas, int indiceTarefa);

#endif
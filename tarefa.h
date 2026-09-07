#ifndef TAREFA_H
#define TAREFA_H

#include <stdio.h>
#include <stdlib.h>
#define NOME_MAX 64

typedef struct Tarefa {
    char nome[NOME_MAX];
    long periodo;
    long deadline;
    long burst;
    long chegadaAtual;
    long deadlineAbs;
    long burstRestante;
    int ativa;
    int completadas;
    int perdidas;
    int killed;
}Tarefa;

void inicializarSimulacao (Tarefa *tarefas, int quantidade);

#endif
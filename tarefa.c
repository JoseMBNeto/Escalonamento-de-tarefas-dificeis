#include "tarefa.h"

void inicializarSimulacao (Tarefa *tarefas, int quantidade){
    for (int i = 0; i < quantidade; i++){
        tarefas[i].chegadaAtual = 0;
        tarefas[i].deadlineAbs = tarefas[i].deadline;
        tarefas[i].burstRestante = tarefas[i].burst;
        tarefas[i].ativa = 1;
        tarefas[i].completadas = 0;
        tarefas[i].perdidas = 0;
        tarefas[i].killed = 0;
    }
}

int precisaNovaInstancia(long instanteAtual, long periodo){
    if (instanteAtual % periodo == 0){
        return 1;
    }
    return 0;
}

void novaInstancia(Tarefa *tarefa, long instanteAtual){
    tarefa->chegadaAtual = instanteAtual;
    tarefa->deadlineAbs = instanteAtual + tarefa->deadline;
    tarefa->burstRestante = tarefa->burst;
    tarefa->ativa = 1;
}
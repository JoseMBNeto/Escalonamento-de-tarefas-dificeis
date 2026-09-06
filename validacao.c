#define _GNU_SOURCE //Usando para o getline funcionar nessa versão do WSL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "validacao.h"

int validarArgumentos (int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Numero de argumentos invalido, escreva 3 argumentos\n");
        return 1;
    }

    if (strcmp(argv[1], "rate") != 0){
        fprintf(stderr, "Erro: tarefa desconhecida\n");
        return 1;
    }

    if (strcmp(argv[1], "edf") != 0){
        fprintf(stderr, "Erro: tarefa desconhecida\n");
        return 1;
    }

    return 0;
}

int validaPositivo (long valor){
    if (valor <= 0){
        fprintf(stderr, "Valor digitado nao e positivo\n");
        return 1;
    }

    return 0;
}

int validaCDP (long c, long d, long p){
    if (c <= d && d <= p){
        return 0;
    }else{
        fprintf(stderr, "Erro: valores de CPD invalidos\n");
        return 1;
    }
}

int validarNumerico (const char *texto, long *saida){
    if (texto == NULL || texto[0] == '\0'){
        fprintf(stderr, "Erro: nada encontrado para leitura\n");
        return 1;
    }

    char *ptrfim;
    errno = 0;
    long valor = strtol(texto, &ptrfim, 10);

    if(ptrfim == texto){
        return 1;
    }

    if (*ptrfim != '\0'){
        return 1;
    }

    if (errno == ERANGE){
        return 1;
    }

    *saida = valor;
    return 0;
}

FILE *abrirArquivo (const char *caminho){
    FILE *arquivo = fopen(caminho, "r");
    
    if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo '%s': %s\n", caminho, strerror(errno));
        return NULL;
    }

    return arquivo;
}

int lerTempoTotal (FILE *arquivo, long *tempoTotal){
    char *linha = NULL;
    size_t tamanho = 0;
    ssize_t lidos = getline(&linha, &tamanho, arquivo);

    if (lidos == -1){
        free(linha);
        return 1;
    }

    if (lidos > 0 && linha[lidos - 1] == '\n'){
        linha[lidos - 1] = '\0';
    }

    int resultado = validarNumerico(linha, tempoTotal);
    free(linha);

    if (resultado != 0){
        return 1;
    }

    if (validaPositivo(*tempoTotal) != 0){
        return 1;
    }

    return 0;
}

int lerLinhaTarefa (const char *linha, char *nomeSaida, char *periodoTexto, char *deadlineTexto, char *burstTexto){
    int campos = sscanf(linha , "%s63 %s63 %s63 %s63", nomeSaida, periodoTexto, deadlineTexto, burstTexto);

    if (campos != 4){
        return 1;
    }

    return 0;
}

Tarefa *lerTarefas (FILE *arquivo, int *quantidadeSaida, int *linhaErroSaida){
    int capacidade = 4;
    int quantidade = 0;
    Tarefa *tarefas = malloc (capacidade * sizeof(Tarefa));

    if (tarefas == NULL){
        *linhaErroSaida = -1;
        return NULL;
    }

    char *linha = NULL;
    size_t tamanhoBuffer = 0;
    int numeroLinha = 2;

    while (getline(&linha, &tamanhoBuffer, arquivo) != -1){
        size_t tamanho = strlen(linha);
        if (tamanho > 0 && linha[tamanho - 1] == '\n'){
            linha[tamanho - 1] = '\0';
        }

        if (linha[0] == '\0'){
            numeroLinha++;
            continue;
        }

        char nomeTexto[NOME_MAX];
        char periodoTexto[NOME_MAX];
        char deadlineTexto[NOME_MAX];
        char burstTexto[NOME_MAX];

        if (lerLinhaTarefa(linha, nomeTexto, periodoTexto, deadlineTexto, burstTexto) != 0){
            *linhaErroSaida = numeroLinha;
            free(linha);
            free(tarefas);
            return NULL;
        }

        long periodo, deadline, burst;

        if (validarNumerico(periodoTexto, &periodo) != 0 || validarNumerico(deadlineTexto, &deadline) != 0 || validarNumerico(burstTexto, &burst) != 0){
            *linhaErroSaida = numeroLinha;
            free(linha);
            free(tarefas);
            return NULL;
        }

        if (validaPositivo(periodo) != 0 || validaPositivo(deadline) != 0 || validaPositivo(burst) != 0){
            *linhaErroSaida = numeroLinha;
            free(linha);
            free(tarefas);
            return NULL;
        }

        if (validaCDP(burst, deadline, periodo) != 0) {
            *linhaErroSaida = numeroLinha;
            free(linha);
            free(tarefas);
            return NULL;
        }

        if (quantidade == capacidade) {
            capacidade *= 2;
            Tarefa *novo = realloc(tarefas, capacidade * sizeof(Tarefa));
            if (novo == NULL) {
                *linhaErroSaida = -1;
                free(linha);
                free(tarefas);
                return NULL;
            }
            tarefas = novo;
        }

        strncpy(tarefas[quantidade].nome, nomeTexto, NOME_MAX - 1);
        tarefas[quantidade].nome[NOME_MAX - 1] = '\0';
        tarefas[quantidade].periodo = periodo;
        tarefas[quantidade].deadline = deadline;
        tarefas[quantidade].burst = burst;

        quantidade++;
        numeroLinha++;
    }
    
    free(linha);

    *quantidadeSaida = quantidade;
    *linhaErroSaida = -1;
    return tarefas;
}
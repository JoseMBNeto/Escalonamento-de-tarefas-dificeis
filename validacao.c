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

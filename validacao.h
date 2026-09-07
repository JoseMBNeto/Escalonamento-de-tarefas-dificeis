#ifndef VALIDACAO_H
#define VALIDACAO_H

#include <stdio.h>
#include "tarefa.h"

int validarArgumentos (int argc, char *argv[]);
FILE *abrirArquivo (const char *caminho);
int lerTempoTotal (FILE *arquivo, long *tempoTotal);
int lerLinhaTarefa (const char *linha, char *nomeSaida, char *periodoTexto, char *deadlineTexto, char *burstTexto);
int validarNumerico (const char *texto, long *saida);
int validaPositivo (long valor);
int validaCDP (long c, long d, long p);
Tarefa *lerTarefas (FILE *arquivo, int *quantidadeSaida, int *linhaErroSaida);
#endif
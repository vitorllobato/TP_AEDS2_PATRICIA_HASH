/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#ifndef COMUM_H
#define COMUM_H

#include "TAD_Hash.h"
#include "Patricia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILE_NAME 100

int comparaRelevancia(const void *a, const void *b);
int contabilizaIngredienteHash(const char *filename, const char *ingrediente);
void transformarMaiuscula(char *str);
void removeEspacoAdicional(char *str);
void processaArquivosHash(const char *filename, int doc_id);
void leArquivoEntradaHash(const char *nome_arq, char lista_arq[][MAX_FILE_NAME], int *num_arq);
void imprimeIndiceInvertidoHash();

#endif
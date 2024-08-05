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

void transformarMaiusculaComum(char *str);
void removeEspacoAdicionalComum(char *str);
void processaArquivosComum(const char *filename, int doc_id);
void leArquivoEntradaComum(const char *filename, char file_list[][MAX_FILE_NAME], int *num_files);

void processaArquivosPatricia(PatriciaNo *root, const char *nome_arq, int doc_id);

#endif
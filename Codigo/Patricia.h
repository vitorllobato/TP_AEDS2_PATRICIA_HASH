/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#ifndef PATRICIA_H
#define PATRICIA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

typedef enum { Interno, Externo } TipoNo;  // enumeração, pode ser tipo Interno ou Externo

typedef struct PatriciaNo {
    TipoNo tipo;  // aqui marca qual o tipo do nó
    union {
        // Caso seja interno:
        struct {
            int index;  // índice do bit discriminador
            struct PatriciaNo *left, *right;  // ponteiro para os filhos
        } interno;

        // Caso seja externo:
        struct {
            char *ingrediente;
            int *doc_ids;  // array de ids dos documentos onde a palavra aparece
            int doc_count;
            int doc_size;  // Tamanho do array doc_ids
        } externo;
    } no;
} PatriciaNo;

PatriciaNo* initPatricia();  // retorna ponteiro pra raiz
PatriciaNo* insertPatricia(PatriciaNo *root, const char *ingrediente, int doc_id);  // insere
PatriciaNo* searchPatricia(PatriciaNo *root, const char *ingrediente);  // busca
void printPatricia(PatriciaNo *root);
void freePatricia(PatriciaNo *root);

// Funções auxiliares
PatriciaNo* criaNoExterno(const char *ingrediente, int doc_id);
PatriciaNo* criaNoInterno(int index, PatriciaNo *left, PatriciaNo *right);
int bitPos(char c, int pos);
int primeiroBitDiferente(const char *a, const char *b);

// Funções adicionais
int comparaRelevanciaPatricia(const void *n1, const void *n2);
int contabilizaIngredientePatricia(PatriciaNo **root, const char *nome_arq, const char *ingrediente, int doc_id);
void transformaMaiuscula(char *str);
void removeEspacoAdicional(char *str);
void printIndiceInvertido(PatriciaNo *root);

#endif

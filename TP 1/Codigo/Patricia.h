#ifndef PATRICIA_H
#define PATRICIA_H

#include "TAD_Comum.h"

typedef enum{Interno, Externo} TipoNo;  //enumeração, pode ser tipo Interno ou Externo
typedef struct PatriciaNo {
    TipoNo tipo;                        //aqui marca qual o ttipo do nó
    union {
        //CAso seja interno:
        struct {
            int index;                          //indice do bit discriminador
            struct PatriciaNo *left, *right;    //ponteiro para os filhos
        }interno;

        //Caso seja externo:
        struct {
            char *ingrediente;
            int *doc_ids;           //array de ids dos documentos onde a palavra aparece
            int doc_count;          //
            int doc_size;           // Tamanho do array doc_ids
        }externo;
    }no;
}PatriciaNo;


PatriciaNo* initPatricia(); //retorna ponteiro pra raiz
PatriciaNo* insertPatricia(PatriciaNo *root, const char *ingrediente, int doc_id); //insere
PatriciaNo* searchPatricia(PatriciaNo *root, const char *ingrediente); //busca

void printPatricia(PatriciaNo *root);
void freePatricia(PatriciaNo *root);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo o tamanho da tabela hash
#define TABLE_SIZE 47             // tamanho maximo da tabela
#define MAX_INGREDIENT_LENGTH 100 // tamanho maximo do nome do ingrediente
#define MAX_FILE_NAME 100         // tamanha maximo do nome do arquivo
#define MAX_LINE_LENGTH 1024      // tamanho maximo da linha 
#define MAX_DOC_COUNT 100         // quantidade maxima q o ingred aparece no documento
#define MAX_FILES 50              // quantidade maxima de arquivos

// ------------------------------ STRUCTS
typedef struct Node {
    char ingrediente[MAX_INGREDIENT_LENGTH];
    int doc_ids[MAX_LINE_LENGTH];   // Lista de IDs dos documentos
    int doc_qtd[MAX_DOC_COUNT];     // Lista de quantidades de cada documento
    int doc_count;                  // Contador de IDs de documentos
    struct Node *prox;
} Node;

typedef struct {
    int doc_id;
    int qtd;
} Relevancia;

Node* tb_hash[TABLE_SIZE];
typedef unsigned Peso[MAX_INGREDIENT_LENGTH];
Peso p;

// ------------------------------ DEFINIR FUNCOES
void GeraPesosHash(Peso p);
int funcaoHash(const char *str);
void inicializaHash();
Node* criarNoHash(const char *ingrediente, int doc_id, int qtd);
void insereHash(const char *ingrediente, int doc_id, int qtd);
int comparaRelevanciaHash(const void *a, const void *b);
void buscaHash(const char *ingrediente);
int contabilizaIngredienteHash(const char *filename, const char *ingrediente);
void transformarMaiuscula(char *str);
void removeEspacoAdicional(char *str);
void processaArquivosHash(const char *filename, int doc_id);
void leArquivoEntradaHash(const char *filename, char file_list[][MAX_FILE_NAME], int *num_files);
void imprimeHash();
void imprimeIndiceInvertidoHash();
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo o tamanho da tabela hash
#define TABLE_SIZE 47             // tamanho maximo da tabela
#define MAX_INGREDIENT_LENGTH 100 // tamanho maximo do ingrediente
#define MAX_FILE_NAME 100         // tamanha maximo do nome
#define MAX_LINE_LENGTH 1024      // tamanho da linha maxima
#define MAX_DOC_COUNT 100         // qtd q a palavra aparece no documento

// ------------------------------ STRUCTS

typedef struct Node {
    char ingredient[MAX_INGREDIENT_LENGTH];
    int doc_ids[MAX_LINE_LENGTH];  // Lista de IDs dos documentos
    int doc_qtd[MAX_DOC_COUNT];  // Lista de quantidades de cada documento
    int doc_count;  // Contador de IDs de documentos
    struct Node *next;
} Node;

Node* hash_table[TABLE_SIZE];
typedef unsigned TipoPesos[MAX_INGREDIENT_LENGTH];
TipoPesos p;

// ------------------------------ DEFINIR FUNCOES
int hash(const char *str);
void init_hash_table();
Node* create_node(const char *ingredient, int doc_id, int qtd);
void insert_ingredient(const char *ingredient, int doc_id, int qtd);
int find_ingredient(const char *ingredient);
void process_file(const char *filename, int doc_id);
void read_file_list(const char *filename, char file_list[][MAX_FILE_NAME], int *num_files);
void print_hash_table();


/*
void print_registros(const RegistroData *data);

void process_file(const char *file_name, RegistroData *data);

FileData read_file(const char *filename);

void inicializarTabela(int tab[]);

int funcaoHash(int chave);

void inserir(int tab[], int valor);

int busca(int tab[], int chave);

void imprimir(int tab[]);
*/
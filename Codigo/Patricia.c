/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Patricia.h"

// Função para inicializar a árvore Patricia
PatriciaNo* initPatricia() {
    return NULL;
}

// Função auxiliar para criar um nó externo
PatriciaNo* criaNoExterno(const char *ingrediente, int doc_id) {
    PatriciaNo *no = (PatriciaNo *)malloc(sizeof(PatriciaNo));
    no->tipo = Externo;
    no->no.externo.ingrediente = strdup(ingrediente);
    no->no.externo.doc_ids = (int *)malloc(sizeof(int));
    no->no.externo.doc_ids[0] = doc_id;
    no->no.externo.doc_count = 1;
    no->no.externo.doc_size = 1;
    return no;
}

// Função auxiliar para criar um nó interno
PatriciaNo* criaNoInterno(int index, PatriciaNo *left, PatriciaNo *right) {
    PatriciaNo *no = (PatriciaNo *)malloc(sizeof(PatriciaNo));
    no->tipo = Interno;
    no->no.interno.index = index;
    no->no.interno.left = left;
    no->no.interno.right = right;
    return no;
}

// Função auxiliar para verificar o bit de um char em uma posição específica
int bitPos(char c, int pos) {
    return (c >> pos) & 1;
}

// Função auxiliar para buscar o primeiro bit diferente entre duas strings
int primeiroBitDiferente(const char *a, const char *b) {
    int i;
    for (i = 0; a[i] == b[i] && a[i] != '\0'; i++);
    return i * 8 + bitPos(a[i], 0) != bitPos(b[i], 0);
}

// Função para inserir um ingrediente na árvore Patricia
PatriciaNo* insertPatricia(PatriciaNo *root, const char *ingrediente, int doc_id) {
    int i;
    if (!root) {
        return criaNoExterno(ingrediente, doc_id);
    }

    PatriciaNo *atual = root;
    PatriciaNo *pai = NULL;
    int index = 0;

    while (atual->tipo == Interno) {
        pai = atual;
        index = atual->no.interno.index;
        atual = bitPos(ingrediente[index / 8], 7 - (index % 8)) ? atual->no.interno.right : atual->no.interno.left;
    }

    if (strcmp(atual->no.externo.ingrediente, ingrediente) == 0) {
        int *doc_ids = atual->no.externo.doc_ids;
        int doc_count = atual->no.externo.doc_count;

        // Verifica se o doc_id já está presente
        int found = 0;
        for (i = 0; i < doc_count; i++) {
            if (doc_ids[i] == doc_id) {
                found = 1;
                break;
            }
        }

        // Adiciona doc_id se não estiver presente
        if (!found) {
            if (atual->no.externo.doc_count == atual->no.externo.doc_size) {
                atual->no.externo.doc_size *= 2;
                atual->no.externo.doc_ids = (int *)realloc(atual->no.externo.doc_ids, atual->no.externo.doc_size * sizeof(int));
            }
            atual->no.externo.doc_ids[atual->no.externo.doc_count++] = doc_id;
        }
        return root;
    }

    int diff_bit = primeiroBitDiferente(atual->no.externo.ingrediente, ingrediente);

    atual = root;
    PatriciaNo *novo = criaNoExterno(ingrediente, doc_id);
    PatriciaNo *novoInterno = criaNoInterno(diff_bit, NULL, NULL);

    while (atual->tipo == Interno && atual->no.interno.index < diff_bit) {
        pai = atual;
        index = atual->no.interno.index;
        atual = bitPos(ingrediente[index / 8], 7 - (index % 8)) ? atual->no.interno.right : atual->no.interno.left;
    }

    if (bitPos(ingrediente[diff_bit / 8], 7 - (diff_bit % 8))) {
        novoInterno->no.interno.left = atual;
        novoInterno->no.interno.right = novo;
    } else {
        novoInterno->no.interno.left = novo;
        novoInterno->no.interno.right = atual;
    }

    if (!pai) {
        return novoInterno;
    } else if (bitPos(ingrediente[index / 8], 7 - (index % 8))) {
        pai->no.interno.right = novoInterno;
    } else {
        pai->no.interno.left = novoInterno;
    }

    return root;
}

// Função para buscar um ingrediente na árvore Patricia
PatriciaNo* searchPatricia(PatriciaNo *root, const char *ingrediente) {
    PatriciaNo *atual = root;

    while (atual && atual->tipo == Interno) {
        int index = atual->no.interno.index;
        atual = bitPos(ingrediente[index / 8], 7 - (index % 8)) ? atual->no.interno.right : atual->no.interno.left;
    }

    if (atual && strcmp(atual->no.externo.ingrediente, ingrediente) == 0) {
        return atual;
    }

    return NULL;
}

// Função para imprimir a árvore Patricia
void printPatricia(PatriciaNo *root) {
    int i;
    if (!root) return;

    if (root->tipo == Externo) {
        printf("Ingrediente: %s\n", root->no.externo.ingrediente);
        for (i = 0; i < root->no.externo.doc_count; i++) {
            printf("  <DocID: %d>\n", root->no.externo.doc_ids[i]);
        }
    } else {
        printPatricia(root->no.interno.left);
        printPatricia(root->no.interno.right);
    }
}

// Função para liberar a memória da árvore Patricia
void freePatricia(PatriciaNo *root) {
    if (!root) return;

    if (root->tipo == Externo) {
        free(root->no.externo.ingrediente);
        free(root->no.externo.doc_ids);
    } else {
        freePatricia(root->no.interno.left);
        freePatricia(root->no.interno.right);
    }
    free(root);
}

// ------------------------------ Funções Adicionais

// Função para comparar relevância de ingredientes na Patricia
int comparaRelevanciaPatricia(const void *n1, const void *n2) {
    PatriciaNo *node1 = *(PatriciaNo **)n1;
    PatriciaNo *node2 = *(PatriciaNo **)n2;

    // Comparação pela quantidade de documentos
    if (node1->no.externo.doc_count != node2->no.externo.doc_count) {
        return node2->no.externo.doc_count - node1->no.externo.doc_count;
    }
    return strcmp(node1->no.externo.ingrediente, node2->no.externo.ingrediente);
}

// Função para contabilizar ingredientes em um arquivo e atualizar a Patricia
int contabilizaIngredientePatricia(PatriciaNo **root, const char *nome_arq, const char *ingrediente, int doc_id) {
    int qtd_palavras = 0;   // Contador para a quantidade de vezes que o ingrediente é encontrado
    int i,j;
    FILE *arq = fopen(nome_arq, "r");
    if (!arq) {
        perror("Erro ao abrir arquivo de receita");
        return 0;
    }

    char linha[MAX_LINE_LENGTH];        // variável para armazenar cada linha do arquivo
    char copia_ingred[MAX_LINE_LENGTH]; // variável para armazenar uma cópia do ingrediente
    strcpy(copia_ingred, ingrediente);  // Copia o ingrediente para a variável

    // Separa as palavras do ingrediente
    char *palavs_ingred[50];            // Array para armazenar as palavras do ingrediente
    int qtd_palavs_ingred = 0;          // Contador para o número de palavras no ingrediente
    char *palavra_separada = strtok(copia_ingred, " "); // Separa as palavras do ingrediente

    while (palavra_separada) {
        palavs_ingred[qtd_palavs_ingred++] = palavra_separada;  // Armazena cada palavra do ingrediente
        palavra_separada = strtok(NULL, " ");
    }

    // Lê cada linha do arquivo
    while (fgets(linha, sizeof(linha), arq)) {
        char *palavras[50];                 // Array para armazenar palavras de uma linha
        int cont_palav = 0;
        palavra_separada = strtok(linha, "; \n");   // Separa as palavras da linha por ;
        while (palavra_separada) {
            palavras[cont_palav++] = palavra_separada;  // Armazena cada palavra da linha
            palavra_separada = strtok(NULL, "; \n");
        }

        // Verifica se a linha contém as palavras do ingrediente
        int match = 1;
        for (i = 0; i < qtd_palavs_ingred; i++) {
            int word_found = 0;
            for (j = 0; j < cont_palav; j++) {
                if (strcasecmp(palavs_ingred[i], palavras[j]) == 0) {
                    word_found = 1;
                    break;
                }
            }
            if (!word_found) {
                match = 0;
                break;
            }
        }

        // Se todas as palavras do ingrediente estão na linha, incrementa o contador e atualiza a árvore
        if (match) {
            qtd_palavras++;
            *root = insertPatricia(*root, ingrediente, doc_id);
        }
    }

    fclose(arq);
    return qtd_palavras;
}

// Função para converter uma string em maiúsculas
void transformaMaiuscula(char *str) {
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Função para remover espaços adicionais de uma string
void removeEspacoAdicional(char *str) {
    char *dest = str;
    char *src = str;
    char last_char = ' ';

    // Remove espaços extras
    while (*src) {
        if (isspace((unsigned char)*src)) {
            if (last_char != ' ') {
                *dest++ = ' ';
                last_char = ' ';
            }
        } else {
            *dest++ = *src;
            last_char = *src;
        }
        src++;
    }

    // Remove espaço final se houver
    if (last_char == ' ' && dest > str) {
        dest--;
    }

    *dest = '\0';
}

void printIndiceInvertido(PatriciaNo *root) {
    if (!root) return;

    if (root->tipo == Externo) {
        printf(" %s ", root->no.externo.ingrediente);
        for (int i = 0; i < root->no.externo.doc_count; i++) {
            int doc_id = root->no.externo.doc_ids[i];
            printf(" | < %d, %d >", 1, doc_id); // Assumindo uma ocorrência por documento
        }
        printf("\n");
    } else {
        printIndiceInvertido(root->no.interno.left);
        printIndiceInvertido(root->no.interno.right);
    }
}
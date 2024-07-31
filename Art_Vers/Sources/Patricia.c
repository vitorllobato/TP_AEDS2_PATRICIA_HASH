#include "Patricia.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


PatriciaNo* initPatricia() {
    return NULL; //arvore vazia
}

//func aux para criar um nó externo
PatriciaNo* criaNoExterno(const char *ingrediente, int doc_id) {
    PatriciaNo *novoNo = (PatriciaNo*) malloc(sizeof(PatriciaNo));
    novoNo->tipo = Externo;
    novoNo->no.externo.ingrediente = strdup(ingrediente);
    novoNo->no.externo.doc_size = 10;
    novoNo->no.externo.doc_ids = (int*) malloc(novoNo->no.externo.doc_size * sizeof(int));
    novoNo->no.externo.doc_ids[0] = doc_id;
    novoNo->no.externo.doc_count = 1;
    return novoNo;
}

//func pra obter o valor do bit de uma string em uma posição
int bit(int k, char *str) {
    int c = k / 8; //encontra byte
    int i = k % 8; //encontra bit dentro do byte
    return ((str[c] >> (7 - i)) & 1); //valor do bit
}

//func aux para verificar se duas strings são diferentes
int diferente(const char *str1, const char *str2) {
    return strcmp(str1, str2) != 0;
}

//func aux para encontrar o índice do primeiro bit diferente
int indicePrimeiraDiferenca(const char *str1, const char *str2) {
    int i = 0;
    while (str1[i] == str2[i]) {
        i++;
    }
    return i * 8; // Retorna o índice do primeiro bit diferente
}

//func para inserir um ingrediente na árvore PATRICIA
PatriciaNo* insertPatricia(PatriciaNo *root, const char *ingrediente, int doc_id) {
    if (root == NULL) {
        return criaNoExterno(ingrediente, doc_id);
    }

    PatriciaNo *parent = NULL;
    PatriciaNo *current = root;
    while (current->tipo == Interno) {
        parent = current;
        if (bit(current->no.interno.index, (char *) ingrediente)) {
            current = current->no.interno.right;
        } else {
            current = current->no.interno.left;
        }
    }

    if (!diferente(current->no.externo.ingrediente, ingrediente)) {
        // O ingrediente já existe, adiciona o doc_id
        if (current->no.externo.doc_count == current->no.externo.doc_size) {
            current->no.externo.doc_size *= 2;
            current->no.externo.doc_ids = (int*) realloc(current->no.externo.doc_ids, current->no.externo.doc_size * sizeof(int));
        }
        current->no.externo.doc_ids[current->no.externo.doc_count++] = doc_id;
        return root;
    }

    int i = indicePrimeiraDiferenca(current->no.externo.ingrediente, ingrediente);
    PatriciaNo *newLeaf = criaNoExterno(ingrediente, doc_id);
    PatriciaNo *newInternal = (PatriciaNo*) malloc(sizeof(PatriciaNo));
    newInternal->tipo = Interno;
    newInternal->no.interno.index = i;
    if (bit(i, (char *) ingrediente)) {
        newInternal->no.interno.left = current;
        newInternal->no.interno.right = newLeaf;
    } else {
        newInternal->no.interno.left = newLeaf;
        newInternal->no.interno.right = current;
    }

    if (parent == NULL) {
        return newInternal;
    } else {
        if (bit(parent->no.interno.index, (char *) ingrediente)) {
            parent->no.interno.right = newInternal;
        } else {
            parent->no.interno.left = newInternal;
        }
    }

    return root;
}

//func de buscar um ingrediente na árvore
PatriciaNo* searchPatricia(PatriciaNo *root, const char *ingrediente) {
    if (root == NULL) {
        return NULL;
    }

    PatriciaNo *current = root;
    while (current->tipo == Interno) {
        if (bit(current->no.interno.index, (char *) ingrediente)) {
            current = current->no.interno.right;
        } else {
            current = current->no.interno.left;
        }
    }

    if (!diferente(current->no.externo.ingrediente, ingrediente)) {
        return current;
    } else {
        return NULL;
    }
}

//func para imprimir a árvore
void printPatriciaRec(PatriciaNo *root) {
    if (root == NULL) {
        return;
    }
    if (root->tipo == Externo) {
        printf("Ingrediente: %s, Documentos: ", root->no.externo.ingrediente);
        for (int i = 0; i < root->no.externo.doc_count; i++) {
            printf("%d ", root->no.externo.doc_ids[i]);
        }
        printf("\n");
    } else {
        printPatriciaRec(root->no.interno.left);
        printPatriciaRec(root->no.interno.right);
    }
}

void printPatricia(PatriciaNo *root) {
    printPatriciaRec(root);
}

//func para liberar a memória alocada pela árvore
void freePatricia(PatriciaNo *root) {
    if (root == NULL) {
        return;
    }
    if (root->tipo == Interno) {
        freePatricia(root->no.interno.left);
        freePatricia(root->no.interno.right);
    } else {
        free(root->no.externo.ingrediente);
        free(root->no.externo.doc_ids);
    }
    free(root);
}

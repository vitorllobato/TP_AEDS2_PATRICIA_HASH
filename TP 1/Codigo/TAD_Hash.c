/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "TAD_Comum.h"
#include "TAD_Hash.h"

// ------------------------------ FUNCAO HASH
void GeraPesosHash(Peso p){
    int i;
    unsigned int semente = (unsigned int) time(NULL);   // Obtém o tempo atual para usar como semente para o gerador de números aleatórios

    srand(semente); // Inicializa o gerador de números aleatórios com a semente obtida
    for(i=0; i<MAX_INGREDIENT_LENGTH; i++){
        p[i] = 1 + rand() % 10000;  // Pesos entre 1 e 10000
    }
}

int funcaoHash(const char *palav){
    int i = 0;
    unsigned int hash = 0;
    int comp = strlen(palav);
    for(i = 0; i < comp; i++){
        hash += (unsigned char)palav[i] * p[i]; // Calcula o valor hash somando o produto do valor ASCII do caractere com o peso correspondente
    }
    return hash % TABLE_SIZE;
}

// ------------------------------ INICIA TABELA
void inicializaHash(){
  int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        tb_hash[i] = NULL;  // Inicializa cada entrada da tabela hash como NULL
    }
}

Node* criarNoHash(const char *ingrediente, int doc_id, int qtd){
    Node *novo_no = (Node*) malloc(sizeof(Node));   // Aloca memória para um novo nó
    if(!novo_no){
        perror("Erro ao alocar memória");   // Imprime uma mensagem de erro se a alocação falhar
        exit(EXIT_FAILURE);
    }
    strncpy(novo_no->ingrediente, ingrediente, MAX_INGREDIENT_LENGTH);  // Copia o nome do ingrediente para o novo nó
    novo_no->doc_ids[0] = doc_id;   // Atribui o ID do documento à primeira posição no array de IDs de documentos do nó
    novo_no->doc_qtd[0] = qtd;  // Atribui a quantidade do ingrediente à primeira posição no array de quantidades de documentos do nó
    novo_no->doc_count = 1;     // Inicializa a contagem de documentos com 1
    novo_no->prox = NULL;   // Define o ponteiro para o próximo nó como NULL
    return novo_no;
}

// ------------------------------ INSERE NA TABELA
void insereHash(const char *ingrediente, int doc_id, int qtd){
  int i;
  unsigned int indice = funcaoHash(ingrediente);    // Calcula o índice na tabela hash para o ingrediente
  Node *atual = tb_hash[indice];    // Obtém o nó atual na posição calculada da tabela hash

  // Verificar se o ingredientee já está na tabela
  while(atual){
      if(strcmp(atual->ingrediente, ingrediente) == 0){   // Compara o ingrediente atual com o ingrediente a ser inserido
          // ---- Verifica se o ID do documento já está na lista
          for(i = 0; i < atual->doc_count; i++){
              if(atual->doc_ids[i] == doc_id){    // Verifica se o ID do documento já está associado a este ingrediente
                  atual->doc_qtd[i] += qtd;  // Atualiza a quantidade do ingrediente para este documento
                  return;  // ID do documento já presente
              }
          }
          // Adiciona o novo ID do documento e quantidade
          atual->doc_ids[atual->doc_count] = doc_id;
          atual->doc_qtd[atual->doc_count++] = qtd;
          return;
      }
      atual = atual->prox;  // Move para o próximo nó na lista encadeada
  }

  // Adiciona um novo ingrediente na tabela hash
  Node *novo_no = criarNoHash(ingrediente, doc_id, qtd);    // Cria um novo nó para o ingrediente
  novo_no->prox = tb_hash[indice];      // Faz o novo nó apontar para o primeiro nó da lista encadeada atual
  tb_hash[indice] = novo_no;        // Atualiza a posição na tabela hash para apontar para o novo nó
}

// ------------------------------ BUSCA NA TABELA
void buscaHash(const char *ingrediente){
    int i, j;
    Node *novo_no = tb_hash[funcaoHash(ingrediente)];   // Calcula o índice na tabela hash para o ingrediente e obtém o nó correspondente
    
    // Lista para armazenar os resultados
    Relevancia relevancias[MAX_DOC_COUNT];      // Array para armazenar os documentos e suas quantidades
    int cont = 0;

    // Percorre a lista encadeada para encontrar o ingrediente
    while(novo_no != NULL){
        if(strcmp(novo_no->ingrediente, ingrediente) == 0){   // Compara o ingrediente atual com o ingrediente buscado
            for (i = 0; i < novo_no->doc_count; i++) {
                relevancias[cont].doc_id = novo_no->doc_ids[i]; // Armazena o ID do documento encontrado
                relevancias[cont].qtd = novo_no->doc_qtd[i];    // Armazena a quantidade do ingrediente para este documento
                cont++; // Incrementa o contador de resultados encontrados
            }
            break;  // Encontrou o ingrediente, não precisa continuar
        }
        novo_no = novo_no->prox;    // Move para o próximo nó na lista encadeada
    }

    if(cont > 0){     // Verifica se encontrou algum documento
        // Ordena os resultados por doc_qtd em ordem decrescente
        //qsort(relevancias, cont, sizeof(Relevancia), comparaRelevanciaHash);

        // Imprime os resultados
        printf("\nIngredientes encontrados:\n");
        for(i = 0; i < cont; i++){
            printf("Id do Documento: %d, Quantidade: %d\n", relevancias[i].doc_id, relevancias[i].qtd);
        }
    }else{
        printf("Ingrediente nao encontrado.\n");
    }
}

// ------------------------------ IMPRIME A TABELA HASH
void imprimeHash(){
  int i,j;
    for (i = 0; i < TABLE_SIZE; ++i) {
        Node *atual = tb_hash[i];
        if (atual == NULL) {
            printf("\nIndice[%d]: Vazio", i);
        } else {
            printf("\nTabela[%d] <qtde,idDoc>:", i);
            while (atual != NULL) {
                printf("\n  Ingrediente: %s", atual->ingrediente);
                for (j = 0; j < atual->doc_count; ++j) {
                    printf(" <%d,%d> ",atual->doc_qtd[j],atual->doc_ids[j]);
                }
                atual = atual->prox;
            }
        }
    }
}

void imprimeIndiceInvertidoHash(){
  int i,j;
    // Cria um array temporário para armazenar todos os ingredientes encontrados na tabela hash
    char *ingredientes[TABLE_SIZE * MAX_FILES];
    int cont = 0;   // Contador para o número de ingredientes armazenados

    // Percorre a tabela hash e armazena os ingredientes no array
    for(i=0; i<TABLE_SIZE; i++){
      Node *atual = tb_hash[i];     // Começa com o início da lista encadeada em cada posição da tabela hash
      while(atual != NULL){
          ingredientes[cont++] = atual->ingrediente;    // Adiciona o ingrediente ao array e incrementa o contador
          atual = atual->prox;      // Move para o próximo nó na lista encadeada
      }
    }

    // Ordena o array de ingredientes em ordem alfabética
    for(i=0; i< cont - 1; i++){
        for(j=i + 1; j<cont; j++){
            if(strcmp(ingredientes[i], ingredientes[j]) > 0){     // Compara dois ingredientes
                // Troca os ingredientes se estiverem na ordem errada
                char *temp = ingredientes[i];
                ingredientes[i] = ingredientes[j];
                ingredientes[j] = temp;
            }
        }
    }

    // Imprime os ingredientes e suas ocorrências na tabela hash
    for(i=0; i<cont; i++){
      Node *atual = tb_hash[funcaoHash(ingredientes[i])];   // Encontra a lista encadeada para o ingrediente atual
      while(atual != NULL){
        if (strcmp(atual->ingrediente, ingredientes[i]) == 0) { // Verifica se o ingrediente é o mesmo
          printf("Ingrediente: %s\n", atual->ingrediente);
          for(j=0; j<atual->doc_count; j++){
              // Imprime a quantidade e o ID do documento para o ingrediente
              printf("  <Qtd: %d, DocID: %d>\n", atual->doc_qtd[j], atual->doc_ids[j]);
          }
        }
        atual = atual->prox;    // Move para o próximo nó na lista encadeada
      }
    }
}
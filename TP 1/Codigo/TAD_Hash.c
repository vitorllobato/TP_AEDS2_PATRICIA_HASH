#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "TAD_Hash.h"

// ------------------------------ FUNCOES PRONTAS
// Função hash

void GeraPesos(TipoPesos p){
    int i;
    unsigned int semente = (unsigned int) time(NULL);

    srand(semente);
    for (i = 0; i < MAX_INGREDIENT_LENGTH; i++)
        p[i] = 1 + rand() % 10000;  // Pesos entre 1 e 10000
}

int hash(const char *str){
    int i = 0;
    unsigned int hash = 0;  // Use unsigned int para evitar problemas de overflow
    int comp = strlen(str);
    for (i = 0; i < comp; i++) {
        hash += (unsigned char)str[i] * p[i];  // Use unsigned char para evitar problemas de sinal
    }
    return hash % TABLE_SIZE;
}

// Inicializa a tabela hash
void init_hash_table() {
  int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

Node* create_node(const char *ingredient, int doc_id, int qtd) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    if (!new_node) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->ingredient, ingredient, MAX_INGREDIENT_LENGTH);
    new_node->doc_ids[0] = doc_id;
    new_node->doc_qtd[0] = qtd;
    new_node->doc_count = 1;
    new_node->next = NULL;
    return new_node;
}

// Insere um ingrediente na tabela hash
void insert_ingredient(const char *ingredient, int doc_id, int qtd) {
  int i;
  unsigned int index = hash(ingredient);
  Node *current = hash_table[index];

  // Verificar se o ingrediente já está na tabela
  while (current) {
      if (strcmp(current->ingredient, ingredient) == 0) {
          printf("\n\nComparando: '%s' - '%s'",current->ingredient,ingredient);
          // Verifica se o ID do documento já está na lista
          for (i = 0; i < current->doc_count; i++) {
              if (current->doc_ids[i] == doc_id) {
                  current->doc_qtd[i] += qtd;  // Atualiza a quantidade
                    printf("\nDOC JA EXISTE Atualizado - indice: %u, Ingrediente: %s, DocID: %d, Quantidade: %d\n", index, ingredient, doc_id, current->doc_qtd[i]);
                  return;  // ID do documento já presente
              }
          }
          // Adiciona o novo ID do documento e quantidade
          current->doc_ids[current->doc_count] = doc_id;
          current->doc_qtd[current->doc_count++] = qtd;
            printf("\nNOVO ID Adicionado - indice: %u, Ingrediente: %s, Novo DocID: %d, Quantidade: %d\n", index, ingredient, doc_id, qtd);
          return;
      }
      current = current->next;
  }

  // Adiciona um novo ingrediente na tabela hash
  Node *new_node = create_node(ingredient, doc_id, qtd);
  new_node->next = hash_table[index];
  hash_table[index] = new_node;
  //printf("\nDOC NN EXISTIA Adicionado - indice: %u, Ingrediente: %s, Novo DocID: %d, Quantidade: %d\n", index, ingredient, doc_id, qtd);
}

int compare_relevance(const void *a, const void *b) {
    Relevance *ra = (Relevance *)a;
    Relevance *rb = (Relevance *)b;

    // Primeiro, ordena por qtd em ordem decrescente
    if (rb->qtd != ra->qtd) {
        return rb->qtd - ra->qtd;
    }

    // Se a quantidade for igual, ordena por doc_id em ordem crescente
    return ra->doc_id - rb->doc_id;   
}

// Função para buscar um ingrediente e imprimir os documentos ordenados por relevância
void find_ingredient(const char *ingredient, char file_list[MAX_FILES][MAX_FILE_NAME]) {
    //printf("\n\n-------'%s'",ingredient);
    int i, j;
    Node *node = hash_table[hash(ingredient)];
    
    // Lista para armazenar os resultados
    Relevance relevances[MAX_DOC_COUNT];
    int count = 0;

    // Percorre a lista encadeada para encontrar o ingrediente
    while (node != NULL) {
        if (strcmp(node->ingredient, ingredient) == 0) {
            for (i = 0; i < node->doc_count; i++) {
                relevances[count].doc_id = node->doc_ids[i];
                relevances[count].qtd = node->doc_qtd[i];
                count++;
            }
            break;  // Encontrou o ingrediente, não precisa continuar
        }
        node = node->next;
    }

    if (count > 0) {
        // Ordena os resultados por doc_qtd em ordem decrescente
        qsort(relevances, count, sizeof(Relevance), compare_relevance);

        // Imprime os resultados
        printf("\nIngredientes encontrados:\n");
        for (i = 0; i < count; i++) {
            printf("Documento: %d, Quantidade: %d\n", relevances[i].doc_id, relevances[i].qtd);
        }
    } else {
        printf("Ingrediente nao encontrado.\n");
    }
}

// Contabiliza a quantidade de vezes que um ingrediente aparece em um arquivo
int contabilizaIngrediente(const char *filename, const char *ingredient){
    int qtd_token = 0;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de receita");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    char *words[50]; // Array para armazenar palavras de uma linha
    int word_count, i;

    // Tokenizar o ingrediente alvo
    char ingredient_copy[MAX_LINE_LENGTH];
    strcpy(ingredient_copy, ingredient);
    char *ingredient_words[50];
    int ingredient_word_count = 0;
    char *token = strtok(ingredient_copy, " ");
    while (token) {
        ingredient_words[ingredient_word_count++] = token;
        token = strtok(NULL, " ");
    }

    while (fgets(line, sizeof(line), file)) {
        // Tokenizar a linha em palavras
        word_count = 0;
        token = strtok(line, "; \n");
        while (token) {
            words[word_count++] = token;
            token = strtok(NULL, "; \n");
        }

        // Comparar palavras consecutivas na linha com o ingrediente
        for (i = 0; i <= word_count - ingredient_word_count; i++) {
          printf("\n");
            int j;
            for (j = 0; j < ingredient_word_count; j++) {
                if (strcmp(words[i + j], ingredient_words[j]) != 0) {
                    printf("Comparando %s com %s\n",ingredient_words[j],words[i + j]);
                    break;
                }
            }
            if (j == ingredient_word_count) { // Ingrediente encontrado
                qtd_token++;
            }
        }
    }

    fclose(file);
    return qtd_token;
}

void to_uppercase(char *str) {
  int i;
    for (i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Função para remover espaços antes e depois de uma string
void removeLeadingSpaces(char *str) {
    char *start = str;
    
    // Avança até o primeiro caractere não-espaco
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    // Move o restante da string para o início
    if (start != str) {
        char *ptr = str;
        while (*start) {
            *ptr++ = *start++;
        }
        *ptr = '\0'; // Adiciona o terminador nulo no final da string
    }
}

void process_file(const char *filename, int doc_id) {
  int qtd_ing = 0;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
      perror("Erro ao abrir arquivo de receita");
      return;
  }

  char line[MAX_LINE_LENGTH];
  char first_line[MAX_LINE_LENGTH] = "";
  char second_line[MAX_LINE_LENGTH] = "";
  char third_line[MAX_LINE_LENGTH] = "";
  int line_number = 0;

  // Ler e armazenar as linhas
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0'; // Remove newline
    if (line_number == 0) {
        // Armazena a primeira linha
        strncpy(first_line, line, MAX_LINE_LENGTH);
    } else if (line_number == 1) {
        // Armazena a segunda linha
        strncpy(second_line, line, MAX_LINE_LENGTH);
    } else if (line_number == 2) {
        // Armazena a terceira linha
        strncpy(third_line, line, MAX_LINE_LENGTH);
        break; // Não precisa ler mais linhas
    }
    line_number++;
  }

  fclose(file);

  to_uppercase(first_line);
  to_uppercase(second_line);
  to_uppercase(third_line);

  
  // Processar a segunda linha
  if (strlen(second_line) > 0) {
    //printf("Linha 2: %s", second_line);
    const char delim[] = ";";
    char *token = strtok(second_line, delim);

    // Processar tokens da segunda linha
    while (token != NULL) {
      int count = 0;

      // Remover ponto final se existir
      size_t len = strlen(token);
      if (token[len - 1] == '.') {
          token[len - 1] = '\0';
      }
      
      to_uppercase(token);
      
      removeLeadingSpaces(token);

      //rSpace(token);
      
      //rSpace(token);

      printf("\n\n\nCHECAR '%s'\n\n",token);

      //printf("Frase: %s\n", token);
      //token[strcspn(token, " ")] = '\0'; // Remove leading/trailing spaces
                
      // Contar ocorrências na primeira linha
      const char *pos1 = first_line;
      while ((pos1 = strstr( pos1, token )) != NULL) {
          count++;
          pos1 += strlen(token);
      }
      
      // Contar ocorrências na terceira linha
      const char *pos3 = third_line;
      while ((pos3 = strstr(pos3, token)) != NULL) {
          count++;
          pos3 += strlen(token);
      }

      //printf("\n\n      ---------token contabilizar: %s\n", token);
      //printf("\n      -----------count: %d\n\n", count++);
      
      // Inserir ingrediente com a quantidade contada
      printf("\n\nINSERIR TOKEN");
      printf("\n--------%s",token);
      printf("\n--------%d",doc_id);
      printf("\n--------%d",count++);
      insert_ingredient(token, doc_id, count++);

      token = strtok(NULL, delim);
    }
  } else {
      printf("Não foi possível ler a segunda linha.\n");
  }

/*
  while (fgets(line, sizeof(line), file)) {
      line_number++;
      char *token = strtok(line, ";");

      while (token) {
        
          // Remove espaços ao redor do token
          while (*token == ' ') token++;
          char *end = token + strlen(token) - 1;
          while (end > token && (*end == ' ' || *end == '\n')) end--;
          *(end + 1) = '\0';

          // Aqui você pode processar a palavra/token
          printf("Word: '%s'\n", token);
          qtd_ing = contabilizaIngrediente(filename, token);

          // Obtém o próximo token
          token = strtok(NULL, ";");
      }
      /*ler o arquivo inteiro e compara as palavras. Conta quantas vezes o token apareceu no arquivo inteiro*/

/*
      if (line_number == 2) {
          char *start = line;
          char *end = line + strlen(line) - 1;

          // Remove espaços ao redor
          while (*start == ' ' || *start == '\n') start++;
          while (end > start && (*end == ' ' || *end == '\n')) end--;
          *(end + 1) = '\0';

          // Processa os ingredientes na linha
          start = line;
          while (start) {
              char *delimiter = strchr(start, ';');
              if (delimiter) {
                  *delimiter = '\0';  // Termina a string no delimitador
                  delimiter++;
              }

              // Remove espaços ao redor do ingrediente
              while (*start == ' ') start++;
              char *ingredient_end = delimiter ? delimiter - 1 : line + strlen(line) - 1;
              while (ingredient_end > start && *ingredient_end == ' ') ingredient_end--;
              *(ingredient_end + 1) = '\0';

              // Adiciona o ingrediente à tabela hash se não estiver presente
              if (*start != '\0') {
                  insert_ingredient(start, doc_id, qtd_ing);  // Considera a quantidade como 1
              }

              start = delimiter;
          }

          break;  // Após processar a segunda linha, não precisa ler mais
      }

  }

  fclose(file);
*/
}

// Função para ler a lista de arquivos
void read_file_list(const char *filename, char file_list[][MAX_FILE_NAME], int *num_files) {
  int i;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de lista");
        return;
    }

    fscanf(file, "%d", num_files);
    fgetc(file);  // Remove o caractere de nova linha após o número de arquivos

    for (i = 0; i < *num_files; i++) {
        fgets(file_list[i], MAX_FILE_NAME, file);
        // Remove o caractere de nova linha
        file_list[i][strcspn(file_list[i], "\n")] = '\0';
    }

    fclose(file);
}

void print_hash_table() {
  int i,j;
    for (i = 0; i < TABLE_SIZE; ++i) {
        Node *current = hash_table[i];
        if (current == NULL) {
            printf("\nIndice[%d]: Vazio", i);
        } else {
            printf("\nTabela[%d] <qtde,idDoc>:", i);
            while (current != NULL) {
                printf("\n  Ingrediente: %s", current->ingredient);
                for (j = 0; j < current->doc_count; ++j) {
                    printf(" <%d,%d> ",current->doc_qtd[j],current->doc_ids[j]);
                }
                current = current->next;
            }
        }
    }
}

// Função para imprimir os índices invertidos em ordem alfabética
void print_inverted_indices() {
  int i,j;
    // Crie um array temporário para armazenar todos os ingredientes
    char *ingredients[TABLE_SIZE * MAX_FILES];
    int count = 0;

    // Percorra a tabela hash e armazene os ingredientes no array
    for (i = 0; i < TABLE_SIZE; i++) {
      Node *current = hash_table[i];
      while (current != NULL) {
          ingredients[count++] = current->ingredient;
          current = current->next;
      }
    }

    // Ordene o array de ingredientes
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (strcmp(ingredients[i], ingredients[j]) > 0) {
                char *temp = ingredients[i];
                ingredients[i] = ingredients[j];
                ingredients[j] = temp;
            }
        }
    }

    // Imprima os ingredientes e suas ocorrências
    for (i = 0; i < count; i++) {
      Node *current = hash_table[hash(ingredients[i])];
      while (current != NULL) {
        if (strcmp(current->ingredient, ingredients[i]) == 0) {
          printf("Ingrediente: %s\n", current->ingredient);
          for (j = 0; j < current->doc_count; j++) {
              printf("  <Qtd: %d, DocID: %d>\n", current->doc_qtd[j], current->doc_ids[j]);
          }
        }
        current = current->next;
      }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
/*
        unsigned int hash(const char *str) {
            unsigned int hash = 0;
            while (*str) {
                hash = (hash * 31) + *str++;
            }
            return hash % TABLE_SIZE;
        }
*/

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
          // Verifica se o ID do documento já está na lista
          for (i = 0; i < current->doc_count; i++) {
              if (current->doc_ids[i] == doc_id) {
                  current->doc_qtd[i] += qtd;  // Atualiza a quantidade
                  return;  // ID do documento já presente
              }
          }
          // Adiciona o novo ID do documento e quantidade
          current->doc_ids[current->doc_count] = doc_id;
          current->doc_qtd[current->doc_count++] = qtd;
          return;
      }
      current = current->next;
  }

  // Adiciona um novo ingrediente na tabela hash
  Node *new_node = create_node(ingredient, doc_id, qtd);
  new_node->next = hash_table[index];
  hash_table[index] = new_node;
}

// Verifica se o ingrediente já está na tabela
int find_ingredient(const char *ingredient) {
    unsigned int index = hash(ingredient);
    Node *current = hash_table[index];
    while (current) {
        if (strcmp(current->ingredient, ingredient) == 0) {
            return 1;  // Ingrediente encontrado
        }
        current = current->next;
    }
    return 0;  // Ingrediente não encontrado
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
    printf("Linha 2: %s", second_line);
    const char delim[] = ";";
    char *token = strtok(second_line, delim);

    to_uppercase(token);
    
    // Processar tokens da segunda linha
    while (token != NULL) {
      int count = 0;

      printf("Frase: %s\n", token);
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

      printf("\n\n      ---------token contabilizar: %s\n", token);
      printf("\n      -----------count: %d\n\n", count++);
      
      // Inserir ingrediente com a quantidade contada
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
            printf("\nTabela[%d]: Vazio", i);
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

/*
void process_file(const char *file_name, RegistroData *data) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo de receita");
    return;
  }

  data->num_registros++;
  data->registros = realloc(data->registros, data->num_registros * sizeof(Registro));
  if (data->registros == NULL) {
    perror("Erro ao realocar memória");
    fclose(file);
    return;
  }

  Registro *reg = &data->registros[data->num_registros - 1];
  reg->idDoc = data->num_registros; // Atribui um ID ao registro
  reg->nome_doc = strdup(file_name);
  reg->ocupado = 1;

  char line[MAX_LINHA];
  if (fgets(line, sizeof(line), file)) {
    strncpy(reg->nome_pocao, line, MAX_NOMEPOCAO);
    reg->nome_pocao[strcspn(reg->nome_pocao, "\n")] = '\0';
  }
  if (fgets(line, sizeof(line), file)) {
    strncpy(reg->ingredientes, line, MAX_INGRED);
    reg->ingredientes[strcspn(reg->ingredientes, "\n")] = '\0';
  }
  if (fgets(line, sizeof(line), file)) {
    strncpy(reg->passos, line, MAX_PASSOS);
    reg->passos[strcspn(reg->passos, "\n")] = '\0';
  }

  fclose(file);
}

void print_registros(const RegistroData *data) {
  int i;
  for (i = 0; i < data->num_registros; i++) {
    Registro *reg = &data->registros[i];
    printf("ID: %d\n", reg->idDoc);
    printf("Documento: %s\n", reg->nome_doc);
    printf("Nome da Pocao: %s\n", reg->nome_pocao);
    printf("Ingredientes: %s\n", reg->ingredientes);
    printf("Passos: %s\n", reg->passos);
    printf("\n");
  }
}

FileData read_file(const char *filename) {
  int i, j;

  FileData data;
  data.num_arq = 0;
  data.file_names = NULL;
  
  FILE *file;
  char line[MAX_FILENAME_LENGTH];
  

  // Abre o arquivo para leitura
  file = fopen(filename, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    return data;
  }

  // Lê a primeira linha e armazena o número
  if (fgets(line, sizeof(line), file) != NULL) {
    data.num_arq = atoi(line);
  }

  // Aloca memória para armazenar os nomes dos arquivos
  data.file_names = malloc(data.num_arq * sizeof(char *));
  if (data.file_names == NULL) {
    perror("Erro ao alocar memória");
    fclose(file);
    return data;
  }

  // Lê as próximas linhas e armazena os nomes dos arquivos
  for (i = 0; i < data.num_arq; i++) {
    if (fgets(line, sizeof(line), file) != NULL) {
      // Remove a nova linha do final da string, se existir
      line[strcspn(line, "\n")] = '\0';
      data.file_names[i] = strdup(line);
      if (data.file_names[i] == NULL) {
        perror("Erro ao duplicar string");
        // Liberar memória alocada antes de sair
        for (j = 0; j < i; j++) {
            free(data.file_names[j]);
        }
        free(data.file_names);
        fclose(file);
        data.file_names = NULL;
        return data;
      }
    }
  }

  // Fecha o arquivo
  fclose(file);

  return data;
}

void inicializarTabela(int tab[]){
  int i;
  for (i = 0; i < TAM_TABELA; i++){
    tab[i] = 0;   // 0 nn faz parte do conjunto de elementos (poderia ser -1)
  }
}

int funcaoHash(int chave){
  return chave % TAM_TABELA;
}

void inserir(int tab[], int valor){
  int id = funcaoHash(valor);
  while(tab[id] != 0){  // a posicao que quer inserir ja esta ocupada
    id = funcaoHash(id +1);
  }
  tab[id] = valor;
}

int busca(int tab[], int chave){
  int id = funcaoHash(chave);
  printf("Indice do valor encontrado: %d\n",id);
  while(tab[id] != 0){
    if(tab[id] == chave){
      return tab[id];
    }else{
      id = funcaoHash(id +1);
    }
  }
  return 0;
}

void imprimir(int tab[]){
  int i;
  for (i = 0; i < TAM_TABELA; i++){
    printf("%d = %d\n",i,tab[i]);
  }
  
}
*/

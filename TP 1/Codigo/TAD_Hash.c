#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
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

// ------------------------------ RELEVANCIA
int comparaRelevanciaHash(const void *palavra1, const void *palavra2){
    Relevancia *rpalavra1 = (Relevancia *)palavra1;     // Converte o ponteiro `palavra1` para um ponteiro do tipo `Relevancia`
    Relevancia *rpalavra2 = (Relevancia *)palavra2;     // Converte o ponteiro `palavra2` para um ponteiro do tipo `Relevancia`

    // Primeiro, ordena por qtd em ordem decrescente
    if(rpalavra2->qtd != rpalavra1->qtd){
        return rpalavra2->qtd - rpalavra1->qtd;     // Se as quantidades forem diferentes, retorna a diferença (ordem decrescente)
    }

    // Se a quantidade for igual, ordena por doc_id em ordem crescente
    return rpalavra1->doc_id - rpalavra2->doc_id;       // Se as quantidades forem iguais, retorna a diferença dos doc_ids (ordem crescente)
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
        qsort(relevancias, cont, sizeof(Relevancia), comparaRelevanciaHash);

        // Imprime os resultados
        printf("\nIngredientes encontrados:\n");
        for(i = 0; i < cont; i++){
            printf("Id do Documento: %d, Quantidade: %d\n", relevancias[i].doc_id, relevancias[i].qtd);
        }
    }else{
        printf("Ingrediente nao encontrado.\n");
    }
}

// ------------------------------ CONTABILIZA OS INGREDIENTES
int contabilizaIngredienteHash(const char *nome_arq, const char *ingrediente){
    int qtd_palavras = 0;   // Contador para a quantidade de vezes que o ingrediente é encontrado
    FILE *arq = fopen(nome_arq, "r");
    if(!arq){
        perror("Erro ao abrir arquivo de receita");
        return 0;
    }

    char linha[MAX_LINE_LENGTH];        // variavel para armazenar cada linha do arquivo
    char *palavras[50];                 // Array para armazenar palavras de uma linha
    int cont_palav, i; 
    char copia_ingred[MAX_LINE_LENGTH]; // variavel para armazenar uma cópia do ingrediente
    char *palavs_ingred[50];            // Array para armazenar as palavras do ingrediente
    int qtd_palavs_ingred = 0;          // Contador para o número de palavras no ingrediente
    char *palavra_separada = strtok(copia_ingred, " "); // Separa as palavras do ingrediente
    
    strcpy(copia_ingred, ingrediente);  // Copia o ingrediente para a variavel

    // Separa as palavras do ingrediente
    while(palavra_separada){
        palavs_ingred[qtd_palavs_ingred++] = palavra_separada;  // Armazena cada palavra do ingrediente
        palavra_separada = strtok(NULL, " ");
    }

    // Lê cada linha do arquivo
    while(fgets(linha, sizeof(linha), arq)){
        cont_palav = 0;
        palavra_separada = strtok(linha, "; \n");   // Separa as palavras da linha por ;
        while(palavra_separada){
            palavras[cont_palav++] = palavra_separada;  // Armazena cada palavra da linha
            palavra_separada = strtok(NULL, "; \n");    
        }

        // Comparar palavras consecutivas na linha com o ingrediente
        for(i=0; i<= cont_palav - qtd_palavs_ingred; i++){
            int j;
            for(j=0; j<qtd_palavs_ingred; j++){   
                if (strcmp(palavras[i + j], palavs_ingred[j]) != 0){   // Compara cada palavra da linha com as palavras do ingrediente
                    break;  // Sai do loop se as palavras não coincidirem
                }
            }
            if(j == qtd_palavs_ingred){ // Se todas as palavras coincidirem
                qtd_palavras++;     // Incrementa o contador de vezes que o ingrediente é encontrado
            }
        }
    }

    fclose(arq);
    return qtd_palavras;
}

// ------------------------------ PROCESSA OS ARQUIVOS DAS RECEITAS
void transformarMaiuscula(char *palav){
  int i;
    // Itera sobre cada caractere da string e o transforma em maiúscula
    for(i = 0; palav[i]; i++){
        palav[i] = toupper((unsigned char)palav[i]);
    }
}

void removeEspacoAdicional(char *palav){
    char *comeco = palav;
    
    // Avança até o primeiro caractere não-espaco
    while(isspace((unsigned char)*comeco)){
        comeco++;
    }
    
    // Move o restante da string para o início
    if(comeco != palav){
        char *ptr = palav;
        while(*comeco){
            *ptr++ = *comeco++;
        }
        *ptr = '\0'; // Adiciona o terminador nulo no final da string
    }
}

void processaArquivosHash(const char *nome_arq, int doc_id){
  int qtd_ing = 0;      // Inicializa a quantidade de ingredientes para o documento
  FILE *arq = fopen(nome_arq, "r");
  if(arq == NULL){
      perror("Erro ao abrir arquivo de receita");   // Imprime uma mensagem de erro se o arquivo não puder ser aberto
      return;
  }

  char linha[MAX_LINE_LENGTH];              // variavel para armazenar cada linha do arquivo
  char prim_linha[MAX_LINE_LENGTH] = "";    // variavel para armazenar a primeira linha
  char seg_linha[MAX_LINE_LENGTH] = "";     // variavel para armazenar a segunda linha
  char terc_linha[MAX_LINE_LENGTH] = "";    // variavel para armazenar a terceira linha
  int num_linha = 0;                        // Contador de linhas

  // Ler e armazenar as linhas
  while(fgets(linha, sizeof(linha), arq)){
    linha[strcspn(linha, "\n")] = '\0'; // Remove nova linha (enter, no caso)
    if(num_linha == 0){
        strncpy(prim_linha, linha, MAX_LINE_LENGTH);    // Armazena a primeira linha
    }else if(num_linha == 1){
        strncpy(seg_linha, linha, MAX_LINE_LENGTH);     // Armazena a segunda linha
    }else if(num_linha == 2){
        strncpy(terc_linha, linha, MAX_LINE_LENGTH);    // Armazena a terceira linha
        break; // Não precisa ler mais linhas
    }
    num_linha++;
  }

  fclose(arq);

  // Converte as linhas em maiusculas
  transformarMaiuscula(prim_linha);
  transformarMaiuscula(seg_linha);
  transformarMaiuscula(terc_linha);

  // Processar a segunda linha
  if(strlen(seg_linha) > 0){
    const char delim[] = ";";
    char *ingred_separado = strtok(seg_linha, delim);   // Separa os ingredientes da segunda linha

    // Processar palavras separadas da segunda linha
    while(ingred_separado != NULL){
      int cont = 0;

      // Remover ponto final se existir
      size_t len = strlen(ingred_separado);
      if(ingred_separado[len - 1] == '.'){
          ingred_separado[len - 1] = '\0';
      }
      
      transformarMaiuscula(ingred_separado);    // Converte o ingrediente para maiúscula
      removeEspacoAdicional(ingred_separado);   // Remove espaços adicionais no início do ingrediente
                
      // Contar ocorrências na primeira linha
      const char *pos1 = prim_linha;
      while((pos1 = strstr( pos1, ingred_separado )) != NULL){
          cont++;
          pos1 += strlen(ingred_separado);
      }
      
      // Contar ocorrências na terceira linha
      const char *pos3 = terc_linha;
      while((pos3 = strstr(pos3, ingred_separado)) != NULL){
          cont++;
          pos3 += strlen(ingred_separado);
      }
      
      // Inserir ingrediente com a quantidade contada
      insereHash(ingred_separado, doc_id, cont+1);
      ingred_separado = strtok(NULL, delim);    // Continua para o próximo ingrediente
    }
  }else{
      printf("Não foi possível ler a segunda linha.\n");
  }
}

void leArquivoEntradaHash(const char *nome_arq, char lista_arq[][MAX_FILE_NAME], int *num_arq){
  int i;
    FILE *arq = fopen(nome_arq, "r");
    if(!arq){
        perror("Erro ao abrir arquivo de lista");   // Imprime uma mensagem de erro se o arquivo não puder ser aberto
        return;
    }

    // Lê o número de arquivos a partir do arquivo de lista
    fscanf(arq, "%d", num_arq);
    fgetc(arq);  // Remove o caractere de nova linha após o número de arquivos

    // Lê os nomes dos arquivos e os armazena em lista_arq
    for(i = 0; i < *num_arq; i++){
        fgets(lista_arq[i], MAX_FILE_NAME, arq);    // Lê uma linha do arquivo, que contém o nome de um arquivo
        lista_arq[i][strcspn(lista_arq[i], "\n")] = '\0';   // Remove o caractere de nova linha
    }

    fclose(arq);
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
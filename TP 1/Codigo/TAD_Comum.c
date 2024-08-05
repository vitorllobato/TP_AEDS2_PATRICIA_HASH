/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#include "TAD_Comum.h"
#include "TAD_Hash.h"


//Funções Comuns
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

void transformarMaiuscula(char *palav){
  int i;
    // Itera sobre cada caractere da string e o transforma em maiúscula
    for(i = 0; palav[i]; i++){
        palav[i] = toupper((unsigned char)palav[i]);
    }
}

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


// ------------------------------ FUNCOES DO TAD_HASH
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

void processaArquivosHash(const char *nome_arq, int doc_id){
  //int qtd_ing = 0;      // Inicializa a quantidade de ingredientes para o documento
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
// ------------------------------ FIM DAS FUNCOES DO TAD_HASH


//Funções de arquivos para Patricia
void processaArquivosPatricia(PatriciaNo *root, const char *nome_arq, int doc_id){
  //int qtd_ing = 0;      // Inicializa a quantidade de ingredientes para o documento
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
        insertPatricia(root, ingred_separado, doc_id);
      ingred_separado = strtok(NULL, delim);    // Continua para o próximo ingrediente
    }
  }else{
      printf("Não foi possível ler a segunda linha.\n");
  }
}


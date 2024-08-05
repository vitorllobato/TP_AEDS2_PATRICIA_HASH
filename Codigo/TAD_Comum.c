/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#include "TAD_Comum.h"
#include "TAD_Hash.h"
#include "Patricia.h"

// ------------------------------ PROCESSA OS ARQUIVOS DAS RECEITAS
void transformarMaiusculaComum(char *palav){
  int i;
    // Itera sobre cada caractere da string e o transforma em maiúscula
    for(i = 0; palav[i]; i++){
        palav[i] = toupper((unsigned char)palav[i]);
    }
}

void removeEspacoAdicionalComum(char *palav){
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
      printf("Tentando abrir o arquivo: '%s'\n", nome_arq);

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
  transformarMaiusculaComum(prim_linha);
  transformarMaiusculaComum(seg_linha);
  transformarMaiusculaComum(terc_linha);

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
      
      transformarMaiusculaComum(ingred_separado);    // Converte o ingrediente para maiúscula
      removeEspacoAdicionalComum(ingred_separado);   // Remove espaços adicionais no início do ingrediente
                
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

        printf("Número de arquivos lidos: %d\n", *num_arq);
    for (i = 0; i < *num_arq; i++) {
        printf("Arquivo %d: %s\n", i + 1, lista_arq[i]);
    }

}




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
  transformarMaiusculaComum(prim_linha);
  transformarMaiusculaComum(seg_linha);
  transformarMaiusculaComum(terc_linha);

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
      
      transformarMaiusculaComum(ingred_separado);    // Converte o ingrediente para maiúscula
      removeEspacoAdicionalComum(ingred_separado);   // Remove espaços adicionais no início do ingrediente
                
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
  printf("Índices invertidos da Arvore Patricia construídos.\n");
}


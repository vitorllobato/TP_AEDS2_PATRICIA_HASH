#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD_Hash.h"
#include "TAD_Patricia.h"

int main(){
  int i;
  char lista_arq[MAX_FILES][MAX_FILE_NAME];
  char caminho_arq[MAX_FILE_NAME];
  int num_files = 0;

  // gera os pesos somente uma vez
  GeraPesosHash(p);
  inicializaHash();

  char opcao;
  char busca_ing[MAX_INGREDIENT_LENGTH];
  do{
    printf("\n\nMENU\n");
    printf("a) Receber o arquivo de entrada com as receitas \n");
    printf("b) Contruir os indices invertidos \n");
    printf("c) Imprimir os indices invertidos, em ordem alfabetica (com as listas de ocorrencias) \n");
    printf("d) Realizar buscas por um ou mais termos (arquivos ordenados por relevancia) \n");
    printf("e) Sair \n");
    scanf("%c", &opcao);
    switch (opcao){
    case 'a':
      // Lê a lista de arquivos
      leArquivoEntradaHash("./ArquivosEntrada/Arquivos/entrada.txt", lista_arq, &num_files);  
      break;
    case 'b':
      // Processa cada arquivo
      for (i = 0; i < num_files; i++) {
        sprintf(caminho_arq, "./ArquivosEntrada/Arquivos/ArquivosEntrada/ArquivosTrabalhoPraticoAEDSIIv2/%s", lista_arq[i]);
        processaArquivosHash(caminho_arq, i+1);
      }

      break;
    case 'c':
      printf("\n\n\nTABELA INDICE INVERTIDO\n");
      imprimeIndiceInvertidoHash();
      break;
    case 'd':
      do{
        fflush(stdin);
        printf("\nDigite '0' para sair da busca e voltar para o menu");
        printf("\nEscreva o nome do ingrediente que quer buscar: ");
        fgets(busca_ing, sizeof(busca_ing), stdin);

        // Remove o caractere de nova linha, se presente
        size_t len = strlen(busca_ing);
        if (len > 0 && busca_ing[len - 1] == '\n') {
          busca_ing[len - 1] = '\0';
        }

        // Converter a busca para caixa alta
        transformarMaiuscula(busca_ing);
        buscaHash(busca_ing);

      } while (strcmp(busca_ing, "0") != 0);
      break;
    default:
      break;
    }
  } while (opcao != 'e');

  return 0;
}


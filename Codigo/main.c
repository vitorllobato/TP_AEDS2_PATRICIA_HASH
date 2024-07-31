#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD_Hash.h"
#include "TAD_Patricia.h"

int main(){
  int i;
  char file_list[MAX_FILES][MAX_FILE_NAME];
  char file_path[MAX_FILE_NAME];
  int num_files = 0;

  GeraPesos(p);

  // Inicializa a tabela hash
  init_hash_table();

  


  // Imprime a tabela hash (opcional)
  print_hash_table();
  
  char opcao;
  char busca_ing[MAX_INGREDIENT_LENGTH];
  do{
    printf("\n\nMENU\n");
    printf("a) Receber o arquivo de entrada com as receitas \n");
    printf("b) Contruir os indices invertidos \n");
    printf("c) Imprimir os indices invertidos, em ordem alfabetica (com as listas de ocorrencias) \n");
    printf("d) Realizar buscas por um ou mais termos (arquivos ordenados por relevancia) \n");
    printf("e) Sair \n");
    scanf("%c",&opcao);
    switch (opcao){
    case 'a':
      // Lê a lista de arquivos
      read_file_list("./ArquivosEntrada/Arquivos/entrada.txt", file_list, &num_files);  

      // Processa cada arquivo
      for (i = 0; i < num_files; i++) {
        sprintf(file_path, "./ArquivosEntrada/Arquivos/ArquivosEntrada/ArquivosTrabalhoPraticoAEDSIIv2/%s", file_list[i]);
        //process_file(file_path, i+1);
      }

      break;
    case 'b':
      // Processa cada arquivo
      for (i = 0; i < num_files; i++) {
        //char file_path[MAX_FILE_NAME];
        //sprintf(file_path, "./ArquivosEntrada/Arquivos/ArquivosEntrada/ArquivosTrabalhoPraticoAEDSIIv2/%s", file_list[i]);
        process_file(file_path, i+1);
      }

      break;
    case 'c':
      printf("\nTABELA NORMAL\n");
      print_hash_table();
      printf("\nTABELA INDICE INVERTIDO\n");
      print_inverted_indices();
      break;
    case 'd':
      do{
        printf("Digite 0 para sair da busca e voltar para o menu");
        printf("Escreva o nome do ingrediente que quer buscar: ");
        scanf("%s",&busca_ing);

        // Converter a busca para caixa alta
        to_uppercase(busca_ing);
        find_ingredient(busca_ing, file_list);
      } while (busca_ing != "0");
      break;
    
    default:
      break;
    }
  } while (opcao != 'e');
  
  

  return 0;

  /*
  int i;
  char file_path[256];
  FileData data = read_file("./ArquivosEntrada/Arquivos/entrada.txt");

  RegistroData registros;
  registros.num_registros = 0;
  registros.registros = NULL;

  if (data.file_names != NULL) {
    printf("Numero de arquivos: %d\n", data.num_arq);
    for (i = 0; i < data.num_arq; i++) {
      sprintf(file_path, "./ArquivosEntrada/Arquivos/ArquivosEntrada/arquivo%d.txt", i+1);
      printf("Arquivo %d: %s\n", i + 1, data.file_names[i]);
      process_file(file_path, &registros);
      free(data.file_names[i]); // Libera a memória de cada string
    }
    free(data.file_names); // Libera a memória do array de strings
  }

  print_registros(&registros);

  // Libera a memória alocada para os registros
  for (i = 0; i < registros.num_registros; i++) {
    free(registros.registros[i].nome_doc);
  }
  free(registros.registros);


  printf("Digite um ingrediente para busca: ");
  return 0;*/
}


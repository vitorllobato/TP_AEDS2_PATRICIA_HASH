#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD_Hash.h"
#include "TAD_Patricia.h"

int main(){
  int i;
  
  GeraPesos(p);
  char file_list[100][MAX_FILE_NAME];
  int num_files = 0;

  // Inicializa a tabela hash
  init_hash_table();

  // Lê a lista de arquivos
  read_file_list("./ArquivosEntrada/Arquivos/entrada.txt", file_list, &num_files);

  // Processa cada arquivo
  for (i = 0; i < num_files; i++) {
      char file_path[MAX_FILE_NAME];
      sprintf(file_path, "./ArquivosEntrada/Arquivos/ArquivosEntrada/ArquivosTrabalhoPraticoAEDSIIv2/%s", file_list[i]);
      process_file(file_path, i+1);
  }

  // Imprime a tabela hash (opcional)
  print_hash_table();


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


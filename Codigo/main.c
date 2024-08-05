/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#include "Patricia.h"
#include "TAD_Hash.h"
#include "TAD_Comum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INGREDIENTE_LENGTH 256

int main() {
    int i;
    char opcao;
    char busca_ing[MAX_INGREDIENT_LENGTH];
    char lista_arq[MAX_FILES][MAX_FILE_NAME];
    char caminho_arq[MAX_FILE_NAME];
    int num_files = 0;

    GeraPesosHash(p);
    inicializaHash();

    PatriciaNo *arvore = initPatricia();
    
    do {
        printf("\n\nMENU\n");
        printf("a) Receber o arquivo de entrada com as receitas \n");
        printf("b) Construir os indices invertidos \n");
        printf("c) Imprimir os indices invertidos, em ordem alfabetica (com as listas de ocorrencias) \n");
        printf("d) HASH -  Realizar buscas por um ou mais termos (arquivos ordenados por relevancia) \n");
        printf("e) PATRICIA - Realizar buscas por um ou mais termos (arquivos ordenados por relevancia) \n");
        printf("f) Sair \n");
        scanf("%c", &opcao);
        getchar();  // Limpa o buffer do teclado
        switch (opcao) {
            case 'a':
                leArquivoEntradaHash("./ArquivosEntrada/entrada.txt", lista_arq, &num_files);
                break;
            case 'b':
                for (i = 0; i < num_files; i++) {
                    sprintf(caminho_arq, "./ArquivosEntrada/Arquivos/%s", lista_arq[i]);
                    processaArquivosHash(caminho_arq, i+1);
                    processaArquivosPatricia(arvore, caminho_arq, i+1);

                }
                break;
            case 'c':
                printf("\n\n\nTABELA HASH IMPRESSA - INDICE INVERTIDO\n");
                imprimeIndiceInvertidoHash();

                printf("\n\n\nARVORE PATRICIA IMPRESSA - INDICE INVERTIDO\n");
                printIndiceInvertido(arvore);
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
                    transformarMaiusculaComum(busca_ing);
                    buscaHash(busca_ing);

                } while (strcmp(busca_ing, "0") != 0);

                break;
            case 'e':       
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
                    transformarMaiusculaComum(busca_ing);
                    PatriciaNo *resultado = searchPatricia(arvore, busca_ing);

                } while (strcmp(busca_ing, "0") != 0);
                break;
            case 'f':       
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 'f');
    // Liberar memória alocada pela árvore Patricia e pela tabela Hash
    freePatricia(arvore);
    //freeHash(tabelaHash);
    return 0;
}








/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Patricia.h"

#define MAX_INGREDIENTE_LENGTH 256

int main() {
    PatriciaNo *arvore = initPatricia();
    int opcao;
    char ingrediente[MAX_INGREDIENTE_LENGTH];
    char nome_arq[MAX_INGREDIENTE_LENGTH];
    int doc_id;
    int qtd_palavras;

    do {
        printf("\nMENU:\n");
        printf("1. Inserir ingrediente em um documento\n");
        printf("2. Buscar ingrediente na árvore Patricia\n");
        printf("3. Exibir todos os ingredientes\n");
        printf("4. Transformar ingrediente para maiúsculas\n");
        printf("5. Remover espaços adicionais\n");
        printf("6. Contabilizar e inserir ingredientes de um arquivo\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();  // Limpa o buffer do teclado

        switch (opcao) {
            case 1:
                printf("Digite o ingrediente: ");
                fgets(ingrediente, MAX_INGREDIENTE_LENGTH, stdin);
                ingrediente[strcspn(ingrediente, "\n")] = '\0';  // Remove o \n

                printf("Digite o ID do documento: ");
                scanf("%d", &doc_id);
                getchar();

                arvore = insertPatricia(arvore, ingrediente, doc_id);
                printf("Ingrediente inserido.\n");
                break;

            case 2:
                printf("Digite o ingrediente para buscar: ");
                fgets(ingrediente, MAX_INGREDIENTE_LENGTH, stdin);
                ingrediente[strcspn(ingrediente, "\n")] = '\0';  // Remove o \n

                PatriciaNo *resultado = searchPatricia(arvore, ingrediente);
                if (resultado) {
                    printf("Ingrediente encontrado: %s\n", resultado->no.externo.ingrediente);
                    for (int i = 0; i < resultado->no.externo.doc_count; i++) {
                        printf("  <DocID: %d>\n", resultado->no.externo.doc_ids[i]);
                    }
                } else {
                    printf("Ingrediente não encontrado.\n");
                }
                break;

            case 3:
                printf("Todos os ingredientes na árvore Patricia:\n");
                printPatricia(arvore);
                break;

            case 4:
                printf("Digite a string para transformar em maiúsculas: ");
                fgets(ingrediente, MAX_INGREDIENTE_LENGTH, stdin);
                ingrediente[strcspn(ingrediente, "\n")] = '\0';  // Remove o \n

                transformaMaiuscula(ingrediente);
                printf("String transformada: %s\n", ingrediente);
                break;

            case 5:
                printf("Digite a string para remover espaços adicionais: ");
                fgets(ingrediente, MAX_INGREDIENTE_LENGTH, stdin);
                ingrediente[strcspn(ingrediente, "\n")] = '\0';  // Remove o \n

                removeEspacoAdicional(ingrediente);
                printf("String com espaços removidos: %s\n", ingrediente);
                break;

            case 6:
                printf("Digite o nome do arquivo de receita: ");
                fgets(nome_arq, MAX_INGREDIENTE_LENGTH, stdin);
                nome_arq[strcspn(nome_arq, "\n")] = '\0';  // Remove o \n

                printf("Digite o ingrediente a contabilizar: ");
                fgets(ingrediente, MAX_INGREDIENTE_LENGTH, stdin);
                ingrediente[strcspn(ingrediente, "\n")] = '\0';  // Remove o \n

                printf("Digite o ID do documento: ");
                scanf("%d", &doc_id);
                getchar();

                qtd_palavras = contabilizaIngredientePatricia(&arvore, nome_arq, ingrediente, doc_id);
                printf("Ingrediente '%s' apareceu %d vezes no arquivo %s e foi inserido na árvore.\n", ingrediente, qtd_palavras, nome_arq);
                break;

            case 7:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 7);

    freePatricia(arvore);
    return 0;
}

*/
/*Arthur Teodoro Borges
Paula Rios Moreira
Vitor Vaconcelos Lobato*/

#include "Patricia.h"
#include "TAD_Hash.h"
#include "TAD_Comum.h"

#define MAX_INGREDIENTE_LENGTH 256
int main() {
    GeraPesosHash(p);
    inicializaHash();
    PatriciaNo *arvore = initPatricia();
    int opcao;
    char lista_arq[MAX_FILES][MAX_FILE_NAME];
    char caminho_arq[MAX_FILE_NAME];
    int num_files = 0;
    
    do {
        printf("\nMENU:\n");
        printf("1. Receber arquivo de entrada\n");
        printf("2. Construir índices invertidos\n");
        printf("3. Imprimir índices invertidos\n");
        printf("4. Realizar busca por termo(s) de busca\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();  // Limpa o buffer do teclado
        switch (opcao) {
            case 1:
                leArquivoEntradaHash("entrada.txt", lista_arq, &num_files);
                printf("Arquivo de entrada recebido.\n");
                break;
            case 2:
                for (int i = 0; i < num_files; i++) {
                    sprintf(caminho_arq, "./ArquivosTrabalhoPraticoAEDSIIv2/%s", lista_arq[i]);
                    processaArquivosHash(lista_arq[i], i);
                    processaArquivosPatricia(arvore, caminho_arq, i);
                    contabilizaIngredientePatricia(&arvore, caminho_arq, "a", i);
                }
                printf("Índices invertidos construídos.\n");
                break;
            case 3:
                // Implementar a impressão dos índices invertidos, contendo as palavras em ordem alfabética, uma por linha, com suas respectivas listas de ocorrências
                // Exemplo:
                // imprimirIndicesInvertidos(arvore, tabelaHash);
                printf("Índices invertidos impressos.\n");
                break;
            case 4:
                printf("Digite o(s) termo(s) de busca: ");
                //fgets(ingrediente, MAX_INGREDIENTE_LENGTH, stdin);
                //ingrediente[strcspn(ingrediente, "\n")] = '\0';  // Remove o \n
                // Implementar a busca por termo(s) de busca nos índices construídos
                // Exemplo:
                // buscarTermo(arvore, tabelaHash, ingrediente);
                printf("Busca realizada.\n");
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 5);
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
#include <stdio.h>
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


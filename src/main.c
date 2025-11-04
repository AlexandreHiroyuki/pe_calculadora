#include "BigInt/BigInt.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  // Setup
  int isFile      = 0;
  int setupOption = 0;

  printf("0. Teclado\n");
  printf("1. Arquivo\n");
  printf("Digite a opção: ");
  scanf("%d", &setupOption);

  if(setupOption == 0) { // Teclado
    isFile = 0;
  } else {
    isFile = 1;
  }

  // Super Loop
  while(1) {
    int option = 0;
    int exit   = 0;

    printf("0. Sair\n");
    printf("1. Adição\n");
    printf("2. Subtração\n");
    printf("3. Multiplicação\n");
    printf("4. Divisão\n");
    printf("Digite a opção: ");
    scanf("%d", &option);

    // Jump table elimina o tempo de busca pela opção na lista O(1)
    switch(option) {
    case 0:
      printf("Saindo...\n");
      exit = 1;
      break;
    case 1: {
      printf("Adição\n");
      printf("Digite o primeiro número: ");
      char num1_str[1000];
      scanf("%s", num1_str);

      printf("Digite o segundo número: ");
      char num2_str[1000];
      scanf("%s", num2_str);

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);

      if(num1 && num2) {
        BigInt *result = bigint_sum(num1, num2);
        printf("Resultado: ");
        bigint_print(result);
        printf("\n");

        bigint_destroy(num1);
        bigint_destroy(num2);
        bigint_destroy(result);
      } else {
        printf("Erro ao criar BigInt\n");
      }
      break;
    }
    case 2: printf("Subtração\n"); break;
    case 3: printf("Multiplicação\n"); break;
    case 4: printf("Divisão\n"); break;
    default: printf("Opção inválida\n"); break;
    }

    if(exit) {
      break;
    }
  }
  return 0;
}

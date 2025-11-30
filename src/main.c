#include "BigInt/BigInt.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  // Setup
  int isFile      = 0;
  int setupOption = 0;

  printf("0. Teclado\n");
  printf("1. Arquivo\n");
  printf("Digite a opcao: ");
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
    printf("1. Adicao\n");
    printf("2. Subtracao\n");
    printf("3. Multiplicacao\n");
    printf("4. Divisao Inteira\n");
    printf("5. Modulo\n");
    printf("Digite a opcao: ");
    scanf("%d", &option);

    // Jump table elimina o tempo de busca pela opção na lista O(1)
    switch(option) {
    
      case 0:
      printf("Saindo...\n");
      exit = 1;
      break;
    
      case 1: {
      printf("Adicao\n");
      printf("Digite o primeiro numero: ");
      char num1_str[1000];
      scanf("%s", num1_str);

      printf("Digite o segundo numero: ");
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
    
    case 2: printf("Subtracao\n"); break;

    case 3: {
      printf("Multiplicacao\n");
      printf("Digite o primeiro numero: ");
      char num1_str[1000];
      scanf("%s", num1_str);

      printf("Digite o segundo numero: ");
      char num2_str[1000];
      scanf("%s", num2_str);

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);

      if(num1 && num2) {
        BigInt *result = bigint_multiplicacao(num1, num2);
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

    case 4: {
      printf("Divisao Inteira\n");
      printf("Digite o primeiro numero: ");
      char num1_str[1000];
      scanf("%s", num1_str);

      printf("Digite o segundo numero: ");
      char num2_str[1000];
      scanf("%s", num2_str);

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);

      if(num1 && num2) {
        BigInt *result = bigint_divisao(num1, num2);
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
    
    case 5: {
      printf("Modulo\n");
      printf("Digite o primeiro numero: ");
      char num1_str[1000];
      scanf("%s", num1_str);

      printf("Digite o segundo numero: ");
      char num2_str[1000];
      scanf("%s", num2_str);

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);
      
      if(num1 && num2) {
        BigInt *result = bigint_mod(num1, num2);
        printf("Resultado: ");
        bigint_print(result);
        printf("\n");
      
        bigint_destroy(num1);
        bigint_destroy(num2);
      } else {
        printf("Erro ao criar BigInt\n");
      }

      break;
    }

    default: printf("Opcao inválida\n"); break;
    }

    if(exit) {
      break;
    }
  }
  return 0;
}

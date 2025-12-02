#include "BigInt/BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ler_numeros(int isFile, const char *input_filename, char *num1_str,
                size_t num1_size, char *num2_str, size_t num2_size) {
  if(isFile) {
    FILE *fin = fopen(input_filename, "r");
    if(fin == NULL) {
      perror("Erro ao abrir arquivo de entrada");
      return 0;
    }
    if(fscanf(fin, "%s %s", num1_str, num2_str) != 2) {
      printf("Erro: arquivo deve conter dois números.\n");
      fclose(fin);
      return 0;
    }
    fclose(fin);
  } else {
    printf("Digite o primeiro numero: ");
    scanf("%s", num1_str);
    printf("Digite o segundo numero: ");
    scanf("%s", num2_str);
  }
  return 1;
}

void escrever_resultado(int isFile, const char *output_filename,
                        const BigInt *resultado) {
  if(isFile) {
    FILE *fout = fopen(output_filename, "w");
    if(fout == NULL) {
      perror("Erro ao abrir arquivo de saída");
      return;
    }
    char *res_str = bigint_to_string(resultado);
    if(res_str != NULL) {
      fprintf(fout, "%s\n", res_str);
      free(res_str);
    }
    fclose(fout);
    printf("Resultado gravado em %s\n", output_filename);
  } else {
    printf("Resultado: ");
    bigint_print(resultado);
    printf("\n");
  }
}

int main(void) {
  // Setup
  int isFile      = 0;
  int setupOption = 0;
  char input_filename[256];
  char output_filename[256];

  printf("0. Teclado\n");
  printf("1. Arquivo\n");
  printf("Digite a opcao: ");
  scanf("%d", &setupOption);

  if(setupOption == 0) { // Teclado
    isFile = 0;
  } else {
    isFile = 1;
    printf("Nome do arquivo de entrada: ");
    scanf("%255s", input_filename);
    printf("Nome do arquivo de saida: ");
    scanf("%255s", output_filename);
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
      char num1_str[1024], num2_str[1024];
      if(!ler_numeros(isFile, input_filename, num1_str, sizeof(num1_str),
                      num2_str, sizeof(num2_str))) {
        break;
      }

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);

      if(num1 && num2) {
        BigInt *result = bigint_sum(num1, num2);
        if(result) {
          escrever_resultado(isFile, output_filename, result);
          bigint_destroy(result);
        }
        bigint_destroy(num1);
        bigint_destroy(num2);
      } else {
        printf("Erro ao criar BigInt\n");
        if(num1) bigint_destroy(num1);
        if(num2) bigint_destroy(num2);
      }
      break;
    }
    
    case 2: {
      printf("Subtracao\n");
      char num1_str[1024], num2_str[1024];
      if(!ler_numeros(isFile, input_filename, num1_str, sizeof(num1_str),
                      num2_str, sizeof(num2_str))) {
        break;
      }

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);

      if(num1 && num2) {
        BigInt *result = bigint_subtract(num1, num2);
        if(result) {
          escrever_resultado(isFile, output_filename, result);
          bigint_destroy(result);
        }
        bigint_destroy(num1);
        bigint_destroy(num2);
      } else {
        printf("Erro ao criar BigInt\n");
        if(num1) bigint_destroy(num1);
        if(num2) bigint_destroy(num2);
      }
      break;
    }

    case 3: {
      printf("Multiplicacao\n");
      char num1_str[1024], num2_str[1024];
      if(!ler_numeros(isFile, input_filename, num1_str, sizeof(num1_str),
                      num2_str, sizeof(num2_str))) {
        break;
      }

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);

      if(num1 && num2) {
        BigInt *result = bigint_multiplicacao(num1, num2);
        if(result) {
          escrever_resultado(isFile, output_filename, result);
          bigint_destroy(result);
        }
        bigint_destroy(num1);
        bigint_destroy(num2);
      } else {
        printf("Erro ao criar BigInt\n");
        if(num1) bigint_destroy(num1);
        if(num2) bigint_destroy(num2);
      }

      break;
    } 

    case 4: {
      printf("Divisao Inteira\n");
      char num1_str[1024], num2_str[1024];
      if(!ler_numeros(isFile, input_filename, num1_str, sizeof(num1_str),
                      num2_str, sizeof(num2_str))) {
        break;
      }

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);

      if(num1 && num2) {
        BigInt *result = bigint_divisao(num1, num2);
        if(result) {
          escrever_resultado(isFile, output_filename, result);
          bigint_destroy(result);
        }
        bigint_destroy(num1);
        bigint_destroy(num2);
      } else {
        printf("Erro ao criar BigInt\n");
        if(num1) bigint_destroy(num1);
        if(num2) bigint_destroy(num2);
      }

      break;
    }
    
    case 5: {
      printf("Modulo\n");
      char num1_str[1024], num2_str[1024];
      if(!ler_numeros(isFile, input_filename, num1_str, sizeof(num1_str),
                      num2_str, sizeof(num2_str))) {
        break;
      }

      BigInt *num1 = bigint_create_from_string(num1_str);
      BigInt *num2 = bigint_create_from_string(num2_str);
      
      if(num1 && num2) {
        BigInt *result = bigint_mod(num1, num2);
        if(result) {
          escrever_resultado(isFile, output_filename, result);
          bigint_destroy(result);
        }
        bigint_destroy(num1);
        bigint_destroy(num2);
      } else {
        printf("Erro ao criar BigInt\n");
        if(num1) bigint_destroy(num1);
        if(num2) bigint_destroy(num2);
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

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
    printf("Digite o primeiro número: ");
    scanf("%s", num1_str);
    printf("Digite o segundo número: ");
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

int main() {
  int  opcao;
  int  isFile = 0;
  char input_filename[256];
  char output_filename[256];

  printf("Deseja ler os números de um arquivo? (1 = sim, 0 = não): ");
  scanf("%d", &isFile);

  if(isFile) {
    printf("Nome do arquivo de entrada: ");
    scanf("%255s", input_filename);
    printf("Nome do arquivo de saída: ");
    scanf("%255s", output_filename);
  }

  while(1) {
    printf("\n--- Calculadora BigInt ---\n");
    printf("1. Soma\n");
    printf("2. Subtração\n");
    printf("3. Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    if(opcao == 3) break;

    char num1_str[1024], num2_str[1024];
    if(!ler_numeros(isFile, input_filename, num1_str, sizeof(num1_str),
                    num2_str, sizeof(num2_str))) {
      continue;
    }

    BigInt *a = bigint_create_from_string(num1_str);
    BigInt *b = bigint_create_from_string(num2_str);

    if(!a || !b) {
      printf("Erro ao criar BigInt.\n");
      if(a) bigint_destroy(a);
      if(b) bigint_destroy(b);
      continue;
    }

    BigInt *resultado = NULL;

    if(opcao == 1) {
      resultado = bigint_sum(a, b);
    } else if(opcao == 2) {
      resultado = bigint_subtract(a, b);
    } else {
      printf("Opção inválida.\n");
    }

    if(resultado) {
      escrever_resultado(isFile, output_filename, resultado);
      bigint_destroy(resultado);
    }

    bigint_destroy(a);
    bigint_destroy(b);
  }

  return 0;
}
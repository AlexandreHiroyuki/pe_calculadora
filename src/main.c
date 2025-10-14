#include <stdio.h>

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
    case 1: printf("Adição\n"); break;
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

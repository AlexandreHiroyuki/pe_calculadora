#include "BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cria um BigInt a partir de uma string
BigInt *bigint_create_from_string(const char *str) {
  if(str == NULL) {
    return NULL;
  }

  // Ignora espaços em branco iniciais
  while(*str == ' ' || *str == '\t') {
    str++;
  }

  // Verifica o sinal
  int sign = 1;
  if(*str == '+') {
    sign = 1;
    str++;
  } else if(*str == '-') {
    sign = -1;
    str++;
  }

  // Ignora zeros à esquerda
  while(*str == '0' && *(str + 1) != '\0') {
    str++;
  }

  // Calcula o comprimento dos dígitos
  const char *start = str;
  while(*str >= '0' && *str <= '9') {
    str++;
  }
  size_t len = str - start;

  if(len == 0 && *start == '0') {
    len = 1;
    str = start;
  }

  if(len == 0) {
    return NULL;
  }

  // Aloca memória
  BigInt *bi = malloc(sizeof(BigInt));
  if(bi == NULL) {
    return NULL;
  }

  bi->digits = linkedlist_create();
  if(bi->digits == NULL) {
    free(bi);
    return NULL;
  }

  bi->sign = (len == 1 && start[0] == '0') ? 1 : sign;

  // Armazena os dígitos em ordem little-endian (LSB primeiro)
  for(size_t i = 0; i < len; i++) {
    linkedlist_append(bi->digits, start[len - 1 - i] - '0');
  }

  return bi;
}

// Cria um BigInt a partir de um inteiro
BigInt *bigint_create_from_int(int num) {
  BigInt *bi = malloc(sizeof(BigInt));
  if(bi == NULL) {
    return NULL;
  }

  bi->digits = linkedlist_create();
  if(bi->digits == NULL) {
    free(bi);
    return NULL;
  }

  if(num == 0) {
    bi->sign = 1;
    linkedlist_append(bi->digits, 0);
    return bi;
  }

  int sign = num >= 0 ? 1 : -1;
  num      = (num < 0) ? -num : num;

  bi->sign = sign;

  // Armazena os dígitos em ordem little-endian
  while(num > 0) {
    linkedlist_append(bi->digits, num % 10);
    num /= 10;
  }

  return bi;
}

// Cria um BigInt vazio com capacidade inicial
BigInt *bigint_create_empty(size_t initial_capacity) {
  BigInt *bi = malloc(sizeof(BigInt));
  if(bi == NULL) {
    return NULL;
  }

  bi->digits = linkedlist_create();
  if(bi->digits == NULL) {
    free(bi);
    return NULL;
  }

  bi->sign = 1;

  // Opcionalmente, pré-aloca alguns zeros se necessário
  if(initial_capacity > 0) {
    for(size_t i = 0; i < initial_capacity; i++) {
      linkedlist_append(bi->digits, 0);
    }
  }

  return bi;
}

// Destrói um BigInt e libera sua memória
void bigint_destroy(BigInt *bi) {
  if(bi == NULL) {
    return;
  }

  if(bi->digits != NULL) {
    linkedlist_destroy(bi->digits);
  }
  free(bi);
}

// Cria uma cópia de um BigInt
BigInt *bigint_copy(const BigInt *bi) {
  if(bi == NULL) {
    return NULL;
  }

  BigInt *copy = malloc(sizeof(BigInt));
  if(copy == NULL) {
    return NULL;
  }

  copy->sign   = bi->sign;
  copy->digits = linkedlist_copy(bi->digits);
  if(copy->digits == NULL) {
    free(copy);
    return NULL;
  }

  return copy;
}


// Compara dois BigInts
// Retorna: 1 se a > b, 0 se a == b, -1 se a < b
int bigint_compare(const BigInt *a, const BigInt *b) {
  if(a == NULL || b == NULL) {
    return 0;
  }

  // Compara os sinais
  if(a->sign > b->sign) {
    return 1;
  }
  if(a->sign < b->sign) {
    return -1;
  }

  // Sinais são iguais, compara os comprimentos
  size_t a_len = linkedlist_length(a->digits);
  size_t b_len = linkedlist_length(b->digits);

  if(a_len > b_len) {
    return a->sign;
  }
  if(a_len < b_len) {
    return -a->sign;
  }

  // Mesmo comprimento, compara dígitos do MSB para o LSB
  for(int i = (int)a_len - 1; i >= 0; i--) {
    char a_digit = linkedlist_get(a->digits, i);
    char b_digit = linkedlist_get(b->digits, i);

    if(a_digit > b_digit) {
      return a->sign;
    }
    if(a_digit < b_digit) {
      return -a->sign;
    }
  }

  return 0;
}

// Soma dois BigInts e retorna o resultado
BigInt *bigint_sum(const BigInt *a, const BigInt *b) {
  if(a == NULL || b == NULL) {
    return NULL;
  }

  // Trata sinais diferentes
  if(a->sign != b->sign) {
    // Isso exigiria lógica de subtração
    // Por enquanto, assuma que ambos são positivos
    // TODO: Implementar soma com sinal corretamente
  }

  // Obtém os comprimentos
  size_t a_len   = linkedlist_length(a->digits);
  size_t b_len   = linkedlist_length(b->digits);
  size_t max_len = (a_len > b_len) ? a_len : b_len;

  BigInt *result = bigint_create_empty(0);
  if(result == NULL) {
    return NULL;
  }

  result->sign = a->sign;

  // Soma os dígitos
  int    carry = 0;
  size_t i     = 0;

  while(i < max_len || carry > 0) {
    int sum = carry;

    if(i < a_len) {
      sum += linkedlist_get(a->digits, i);
    }
    if(i < b_len) {
      sum += linkedlist_get(b->digits, i);
    }

    linkedlist_append(result->digits, sum % 10);
    carry = sum / 10;
    i++;
  }

  // Remove zeros à esquerda
  linkedlist_remove_leading_zeros(result->digits);

  return result;
}

// Converte BigInt para string (quem chamar deve liberar o resultado)
char *bigint_to_string(const BigInt *bi) {
  if(bi == NULL) {
    return NULL;
  }

  size_t len = linkedlist_length(bi->digits);

  // Aloca espaço para o sinal e dígitos, mais o terminador nulo
  char *str = malloc((len + 2) * sizeof(char));
  if(str == NULL) {
    return NULL;
  }

  int index = 0;

  // Adiciona o sinal se for negativo
  if(bi->sign == -1) {
    str[index++] = '-';
  }

  // Copia os dígitos em ordem reversa (MSB para LSB)
  for(int i = (int)len - 1; i >= 0; i--) {
    str[index++] = linkedlist_get(bi->digits, i) + '0';
  }

  str[index] = '\0';

  return str;
}

// Converte BigInt para string e imprime
void bigint_print(const BigInt *bi) {
  char *str = bigint_to_string(bi);
  if(str != NULL) {
    printf("%s", str);
    free(str);
  }
}

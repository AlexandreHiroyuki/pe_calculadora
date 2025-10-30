#include "BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

// Cria um BigInt a partir de uma string
BigInt* bigint_create_from_string(const char* str) {
  if (str == NULL) {
    return NULL;
  }

  // Ignora espaços em branco iniciais
  while (*str == ' ' || *str == '\t') {
    str++;
  }

  // Verifica o sinal
  int sign = 1;
  if (*str == '+') {
    sign = 1;
    str++;
  } else if (*str == '-') {
    sign = -1;
    str++;
  }

  // Ignora zeros à esquerda
  while (*str == '0' && *(str + 1) != '\0') {
    str++;
  }

  // Calcula o comprimento dos dígitos
  const char* start = str;
  while (*str >= '0' && *str <= '9') {
    str++;
  }
  size_t len = str - start;

  if (len == 0 && *start == '0') {
    len = 1;
    str = start;
  }

  if (len == 0) {
    return NULL;
  }

  // Aloca memória
  BigInt* bi = malloc(sizeof(BigInt));
  if (bi == NULL) {
    return NULL;
  }

  bi->capacity = len + 5;
  bi->digits = malloc(bi->capacity * sizeof(char));
  if (bi->digits == NULL) {
    free(bi);
    return NULL;
  }

  bi->length = len;
  bi->sign = (len == 1 && start[0] == '0') ? 1 : sign;

  // Armazena os dígitos em ordem little-endian (LSB primeiro)
  for (size_t i = 0; i < len; i++) {
    bi->digits[i] = start[len - 1 - i] - '0';
  }

  // Inicializa a capacidade restante
  for (size_t i = len; i < bi->capacity; i++) {
    bi->digits[i] = 0;
  }

  return bi;
}

// Cria um BigInt a partir de um inteiro
BigInt* bigint_create_from_int(int num) {
  BigInt* bi = malloc(sizeof(BigInt));
  if (bi == NULL) {
    return NULL;
  }

  if (num == 0) {
    bi->capacity = INITIAL_CAPACITY;
    bi->digits = malloc(bi->capacity * sizeof(char));
    if (bi->digits == NULL) {
      free(bi);
      return NULL;
    }

    bi->length = 1;
    bi->sign = 1;
    bi->digits[0] = 0;
    for (size_t i = 1; i < bi->capacity; i++) {
      bi->digits[i] = 0;
    }
    return bi;
  }

  int sign = num >= 0 ? 1 : -1;
  num = (num < 0) ? -num : num;

  // Conta os dígitos
  int temp = num;
  size_t len = 0;
  while (temp > 0) {
    temp /= 10;
    len++;
  }

  bi->capacity = len + 5;
  bi->digits = malloc(bi->capacity * sizeof(char));
  if (bi->digits == NULL) {
    free(bi);
    return NULL;
  }

  bi->length = len;
  bi->sign = sign;

  // Armazena os dígitos em ordem little-endian
  for (size_t i = 0; i < len; i++) {
    bi->digits[i] = num % 10;
    num /= 10;
  }

  // Inicializa a capacidade restante
  for (size_t i = len; i < bi->capacity; i++) {
    bi->digits[i] = 0;
  }

  return bi;
}

// Cria um BigInt vazio com capacidade inicial
BigInt* bigint_create_empty(size_t initial_capacity) {
  if (initial_capacity == 0) {
    initial_capacity = INITIAL_CAPACITY;
  }

  BigInt* bi = malloc(sizeof(BigInt));
  if (bi == NULL) {
    return NULL;
  }

  bi->capacity = initial_capacity;
  bi->digits = malloc(bi->capacity * sizeof(char));
  if (bi->digits == NULL) {
    free(bi);
    return NULL;
  }

  bi->length = 0;
  bi->sign = 1;

  for (size_t i = 0; i < bi->capacity; i++) {
    bi->digits[i] = 0;
  }

  return bi;
}

// Destrói um BigInt e libera sua memória
void bigint_destroy(BigInt* bi) {
  if (bi == NULL) {
    return;
  }

  if (bi->digits != NULL) {
    free(bi->digits);
  }
  free(bi);
}

// Cria uma cópia de um BigInt
BigInt* bigint_copy(const BigInt* bi) {
  if (bi == NULL) {
    return NULL;
  }

  BigInt* copy = malloc(sizeof(BigInt));
  if (copy == NULL) {
    return NULL;
  }

  copy->capacity = bi->capacity;
  copy->length = bi->length;
  copy->sign = bi->sign;

  copy->digits = malloc(copy->capacity * sizeof(char));
  if (copy->digits == NULL) {
    free(copy);
    return NULL;
  }

  for (size_t i = 0; i < bi->capacity; i++) {
    copy->digits[i] = bi->digits[i];
  }

  return copy;
}

// Redimensiona a capacidade de um BigInt
void bigint_resize(BigInt* bi, size_t new_capacity) {
  if (bi == NULL || new_capacity < bi->length) {
    return;
  }

  char* new_digits = realloc(bi->digits, new_capacity * sizeof(char));
  if (new_digits == NULL) {
    return;
  }

  bi->digits = new_digits;

  // Inicializa a nova capacidade com zero
  for (size_t i = bi->capacity; i < new_capacity; i++) {
    bi->digits[i] = 0;
  }

  bi->capacity = new_capacity;
}

// Compara dois BigInts
// Retorna: 1 se a > b, 0 se a == b, -1 se a < b
int bigint_compare(const BigInt* a, const BigInt* b) {
  if (a == NULL || b == NULL) {
    return 0;
  }

  // Compara os sinais
  if (a->sign > b->sign) {
    return 1;
  }
  if (a->sign < b->sign) {
    return -1;
  }

  // Sinais são iguais, compara os comprimentos
  if (a->length > b->length) {
    return a->sign;
  }
  if (a->length < b->length) {
    return -a->sign;
  }

  // Mesmo comprimento, compara dígitos do MSB para o LSB
  for (int i = a->length - 1; i >= 0; i--) {
    if (a->digits[i] > b->digits[i]) {
      return a->sign;
    }
    if (a->digits[i] < b->digits[i]) {
      return -a->sign;
    }
  }

  return 0;
}

// Soma dois BigInts e retorna o resultado
BigInt* bigint_add(const BigInt* a, const BigInt* b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  // Trata sinais diferentes
  if (a->sign != b->sign) {
    // Isso exigiria lógica de subtração
    // Por enquanto, assuma que ambos são positivos
    // TODO: Implementar soma com sinal corretamente
  }

  // Garante capacidade suficiente
  size_t max_len = (a->length > b->length) ? a->length : b->length;
  size_t result_capacity = max_len + 10;  // Espaço extra para o vai-um

  BigInt* result = bigint_create_empty(result_capacity);
  if (result == NULL) {
    return NULL;
  }

  result->sign = a->sign;

  // Soma os dígitos
  int carry = 0;
  size_t i = 0;

  while (i < max_len || carry > 0) {
    int sum = carry;

    if (i < a->length) {
      sum += a->digits[i];
    }
    if (i < b->length) {
      sum += b->digits[i];
    }

    result->digits[i] = sum % 10;
    carry = sum / 10;
    i++;
  }

  result->length = i;

  // Remove zeros à esquerda
  while (result->length > 1 && result->digits[result->length - 1] == 0) {
    result->length--;
  }

  return result;
}

// Converte BigInt para string (quem chamar deve liberar o resultado)
char* bigint_to_string(const BigInt* bi) {
  if (bi == NULL) {
    return NULL;
  }

  // Aloca espaço para o sinal e dígitos, mais o terminador nulo
  char* str = malloc((bi->length + 2) * sizeof(char));
  if (str == NULL) {
    return NULL;
  }

  int index = 0;

  // Adiciona o sinal se for negativo
  if (bi->sign == -1) {
    str[index++] = '-';
  }

  // Copia os dígitos em ordem reversa (MSB para LSB)
  for (int i = bi->length - 1; i >= 0; i--) {
    str[index++] = bi->digits[i] + '0';
  }

  str[index] = '\0';

  return str;
}

// Converte BigInt para string e imprime
void bigint_print(const BigInt* bi) {
  char* str = bigint_to_string(bi);
  if (str != NULL) {
    printf("%s", str);
    free(str);
  }
}


#ifndef BIGINT_H
#define BIGINT_H

#include <stddef.h>

// Estrutura BigInt que armazena dígitos dinamicamente
typedef struct {
  char* digits;         // Array de dígitos (0-9) armazenados como caracteres
  size_t length;        // Quantidade de dígitos
  int sign;             // 1 para positivo, -1 para negativo
  size_t capacity;      // Capacidade atual do array de dígitos
} BigInt;

// Cria um BigInt a partir de uma string
BigInt* bigint_create_from_string(const char* str);

// Cria um BigInt a partir de um inteiro
BigInt* bigint_create_from_int(int num);

// Cria um BigInt vazio com capacidade inicial
BigInt* bigint_create_empty(size_t initial_capacity);

// Destrói um BigInt e libera sua memória
void bigint_destroy(BigInt* bi);

// Cria uma cópia de um BigInt
BigInt* bigint_copy(const BigInt* bi);

// Soma dois BigInts e retorna o resultado
BigInt* bigint_sum(const BigInt* a, const BigInt* b);

// Compara dois BigInts
// Retorna: 1 se a > b, 0 se a == b, -1 se a < b
int bigint_compare(const BigInt* a, const BigInt* b);

// Converte BigInt para string (quem chamar deve liberar o resultado)
char* bigint_to_string(const BigInt* bi);

// Converte BigInt para string e imprime
void bigint_print(const BigInt* bi);

// Redimensiona a capacidade de um BigInt
void bigint_resize(BigInt* bi, size_t new_capacity);

#endif // BIGINT_H


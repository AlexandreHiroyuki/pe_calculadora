#ifndef BIGINT_H
#define BIGINT_H

#include <stddef.h>
#include <stdint.h>
#include "../LinkedList/LinkedList.h"

// Estrutura BigInt que armazena dígitos dinamicamente usando LinkedList
// Cada nó armazena um valor de 32 bits (base 2^32)
typedef struct {
  LinkedList* digits;   // Lista encadeada de valores (base 2^32) armazenados como uint32_t
  int sign;             // 1 para positivo, -1 para negativo
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

#endif // BIGINT_H


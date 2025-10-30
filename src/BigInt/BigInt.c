#include "BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 8

// Helpers internos
static int bigint_is_zero(const BigInt* bi) {
  return bi->length == 0 || (bi->length == 1 && bi->digits[0] == 0);
}

static void bigint_normalize_length(BigInt* bi) {
  while (bi->length > 1 && bi->digits[bi->length - 1] == 0) {
    bi->length--;
  }
  if (bi->length == 0) {
    bi->length = 1;
    bi->digits[0] = 0;
    bi->sign = 1;
  }
}

static void bigint_ensure_capacity(BigInt* bi, size_t min_capacity) {
  if (bi->capacity >= min_capacity) return;
  size_t new_capacity = bi->capacity ? bi->capacity : INITIAL_CAPACITY;
  while (new_capacity < min_capacity) new_capacity *= 2;
  uint32_t* new_digits = realloc(bi->digits, new_capacity * sizeof(uint32_t));
  if (new_digits == NULL) return;
  // zera espaço novo
  for (size_t i = bi->capacity; i < new_capacity; i++) new_digits[i] = 0;
  bi->digits = new_digits;
  bi->capacity = new_capacity;
}

static void bigint_mul_small(BigInt* bi, uint32_t m) {
  if (m == 0 || bigint_is_zero(bi)) {
    bi->length = 1;
    bi->digits[0] = 0;
    bi->sign = 1;
    return;
  }
  uint64_t carry = 0;
  for (size_t i = 0; i < bi->length; i++) {
    uint64_t prod = (uint64_t)bi->digits[i] * m + carry;
    bi->digits[i] = (uint32_t)prod;
    carry = prod >> 32;
  }
  if (carry) {
    bigint_ensure_capacity(bi, bi->length + 1);
    bi->digits[bi->length++] = (uint32_t)carry;
  }
}

static void bigint_add_small(BigInt* bi, uint32_t a) {
  uint64_t sum = (uint64_t)bi->digits[0] + a;
  bi->digits[0] = (uint32_t)sum;
  uint64_t carry = sum >> 32;
  size_t i = 1;
  while (carry && i < bi->length) {
    uint64_t s = (uint64_t)bi->digits[i] + carry;
    bi->digits[i] = (uint32_t)s;
    carry = s >> 32;
    i++;
  }
  if (carry) {
    bigint_ensure_capacity(bi, bi->length + 1);
    bi->digits[bi->length++] = (uint32_t)carry;
  }
}

static uint32_t bigint_div_small(BigInt* bi, uint32_t d) {
  // Divide por d em lugar; retorna resto
  uint64_t rem = 0;
  for (int i = (int)bi->length - 1; i >= 0; i--) {
    uint64_t cur = (rem << 32) | bi->digits[i];
    uint32_t q = (uint32_t)(cur / d);
    rem = cur % d;
    bi->digits[i] = q;
  }
  bigint_normalize_length(bi);
  return (uint32_t)rem;
}

// Cria um BigInt vazio com capacidade inicial
BigInt* bigint_create_empty(size_t initial_capacity) {
  if (initial_capacity == 0) {
    initial_capacity = INITIAL_CAPACITY;
  }
  BigInt* bi = malloc(sizeof(BigInt));
  if (bi == NULL) return NULL;
  bi->capacity = initial_capacity;
  bi->digits = calloc(bi->capacity, sizeof(uint32_t));
  if (bi->digits == NULL) { free(bi); return NULL; }
  bi->length = 1;
  bi->digits[0] = 0;
  bi->sign = 1;
  return bi;
}

// Destrói um BigInt e libera sua memória
void bigint_destroy(BigInt* bi) {
  if (bi == NULL) return;
  if (bi->digits) free(bi->digits);
  free(bi);
}

// Cria uma cópia de um BigInt
BigInt* bigint_copy(const BigInt* bi) {
  if (!bi) return NULL;
  BigInt* copy = malloc(sizeof(BigInt));
  if (!copy) return NULL;
  copy->capacity = bi->capacity;
  copy->length = bi->length;
  copy->sign = bi->sign;
  copy->digits = malloc(copy->capacity * sizeof(uint32_t));
  if (!copy->digits) { free(copy); return NULL; }
  for (size_t i = 0; i < copy->capacity; i++) copy->digits[i] = bi->digits[i];
  return copy;
}

// Redimensiona a capacidade de um BigInt
void bigint_resize(BigInt* bi, size_t new_capacity) {
  if (bi == NULL || new_capacity < bi->length) return;
  uint32_t* new_digits = realloc(bi->digits, new_capacity * sizeof(uint32_t));
  if (!new_digits) return;
  for (size_t i = bi->capacity; i < new_capacity; i++) new_digits[i] = 0;
  bi->digits = new_digits;
  bi->capacity = new_capacity;
}

// Cria um BigInt a partir de um inteiro
BigInt* bigint_create_from_int(int num) {
  BigInt* bi = bigint_create_empty(INITIAL_CAPACITY);
  if (!bi) return NULL;
  if (num == 0) return bi;
  bi->sign = (num < 0) ? -1 : 1;
  uint32_t absn = (uint32_t)((num < 0) ? - (int64_t)num : num);
  bi->digits[0] = absn;
  bi->length = 1;
  return bi;
}

// Cria um BigInt a partir de uma string
BigInt* bigint_create_from_string(const char* str) {
  if (!str) return NULL;
  while (*str == ' ' || *str == '\t') str++;
  int sign = 1;
  if (*str == '+') { sign = 1; str++; }
  else if (*str == '-') { sign = -1; str++; }
  while (*str == '0' && *(str + 1) >= '0' && *(str + 1) <= '9') str++;
  if (*str < '0' || *str > '9') {
    if (*str == '0') {
      return bigint_create_empty(INITIAL_CAPACITY);
    }
    return NULL;
  }
  BigInt* bi = bigint_create_empty(INITIAL_CAPACITY);
  if (!bi) return NULL;
  bi->length = 1;
  bi->digits[0] = 0;
  for (; *str >= '0' && *str <= '9'; str++) {
    bigint_mul_small(bi, 10u);
    bigint_add_small(bi, (uint32_t)(*str - '0'));
  }
  bi->sign = bigint_is_zero(bi) ? 1 : sign;
  bigint_normalize_length(bi);
  return bi;
}

// Compara dois BigInts
// Retorna: 1 se a > b, 0 se a == b, -1 se a < b
int bigint_compare(const BigInt* a, const BigInt* b) {
  if (!a || !b) return 0;
  if (a->sign > b->sign) return 1;
  if (a->sign < b->sign) return -1;
  if (a->length > b->length) return a->sign;
  if (a->length < b->length) return -a->sign;
  for (int i = (int)a->length - 1; i >= 0; i--) {
    if (a->digits[i] > b->digits[i]) return a->sign;
    if (a->digits[i] < b->digits[i]) return -a->sign;
  }
  return 0;
}

// Soma dois BigInts e retorna o resultado (assume mesmo sinal)
BigInt* bigint_add(const BigInt* a, const BigInt* b) {
  if (!a || !b) return NULL;
  if (a->sign != b->sign) {
    // Não implementado: subtração
    return NULL;
  }
  size_t max_len = (a->length > b->length) ? a->length : b->length;
  BigInt* r = bigint_create_empty(max_len + 1);
  if (!r) return NULL;
  r->length = max_len;
  r->sign = a->sign;
  uint64_t carry = 0;
  for (size_t i = 0; i < max_len; i++) {
    uint64_t av = (i < a->length) ? a->digits[i] : 0;
    uint64_t bv = (i < b->length) ? b->digits[i] : 0;
    uint64_t sum = av + bv + carry;
    r->digits[i] = (uint32_t)sum;
    carry = sum >> 32;
  }
  if (carry) {
    r->digits[r->length++] = (uint32_t)carry;
  }
  bigint_normalize_length(r);
  return r;
}

// Converte BigInt para string (quem chamar deve liberar o resultado)
char* bigint_to_string(const BigInt* bi) {
  if (!bi) return NULL;
  if (bigint_is_zero(bi)) {
    char* z = malloc(2);
    if (!z) return NULL;
    z[0] = '0'; z[1] = '\0';
    return z;
  }
  BigInt* tmp = bigint_copy(bi);
  if (!tmp) return NULL;
  tmp->sign = 1; // trabalhar com valor absoluto
  const uint32_t base10 = 1000000000u; // 10^9
  size_t chunks_capacity = 8;
  size_t chunks_len = 0;
  uint32_t* chunks = malloc(chunks_capacity * sizeof(uint32_t));
  if (!chunks) { bigint_destroy(tmp); return NULL; }
  while (!bigint_is_zero(tmp)) {
    uint32_t rem = bigint_div_small(tmp, base10);
    if (chunks_len == chunks_capacity) {
      chunks_capacity *= 2;
      uint32_t* nc = realloc(chunks, chunks_capacity * sizeof(uint32_t));
      if (!nc) { free(chunks); bigint_destroy(tmp); return NULL; }
      chunks = nc;
    }
    chunks[chunks_len++] = rem;
  }
  // Estima tamanho: primeira parcela até 10 dígitos + (chunks_len-1)*9 + sinal + nulo
  size_t buf_size = 1 + 10 + (chunks_len ? (chunks_len - 1) * 9 : 0) + 2;
  char* out = malloc(buf_size);
  if (!out) { free(chunks); bigint_destroy(tmp); return NULL; }
  size_t pos = 0;
  if (bi->sign < 0) out[pos++] = '-';
  // escreve do último chunk para o primeiro
  int last = (int)chunks_len - 1;
  pos += sprintf(out + pos, "%u", chunks[last]);
  for (int i = last - 1; i >= 0; i--) {
    // zero-pad 9 dígitos
    pos += sprintf(out + pos, "%09u", chunks[i]);
  }
  out[pos] = '\0';
  free(chunks);
  bigint_destroy(tmp);
  return out;
}

// Converte BigInt para string e imprime
void bigint_print(const BigInt* bi) {
  char* s = bigint_to_string(bi);
  if (s) { printf("%s", s); free(s); }
}


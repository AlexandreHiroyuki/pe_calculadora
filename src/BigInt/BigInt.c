#include "BigInt.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Divide uma string decimal por um divisor e retorna o resto
// Modifica a string in-place com o quociente
static uint32_t divide_decimal_string_by_uint32(char *str, size_t len,
                                                uint64_t divisor) {
  uint64_t remainder = 0;

  for(size_t i = 0; i < len; i++) {
    remainder = remainder * 10 + (str[i] - '0');
    str[i]    = (char)(remainder / divisor) + '0';
    remainder %= divisor;
  }

  return (uint32_t)remainder;
}

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
  const char *digits_start = str;
  while(*str >= '0' && *str <= '9') {
    str++;
  }
  size_t len = str - digits_start;

  if(len == 0 && *digits_start == '0') {
    len = 1;
    str = digits_start;
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

  bi->sign = (len == 1 && digits_start[0] == '0') ? 1 : sign;

  // Se for zero, apenas adiciona um zero
  if(len == 1 && digits_start[0] == '0') {
    linkedlist_append(bi->digits, 0);
    return bi;
  }

  // Converte string decimal para base 2^32
  // Fazemos divisões sucessivas por 2^32 até que o número seja zero
  char *working_str = malloc(len + 1);
  if(working_str == NULL) {
    linkedlist_destroy(bi->digits);
    free(bi);
    return NULL;
  }

  memcpy(working_str, digits_start, len);
  working_str[len] = '\0';

  size_t working_len = len;

  while(working_len > 0 && !(working_len == 1 && working_str[0] == '0')) {
    uint64_t divisor = ((uint64_t)UINT32_MAX) + 1ULL; // 2^32
    uint32_t remainder =
        divide_decimal_string_by_uint32(working_str, working_len, divisor);
    linkedlist_append(bi->digits, remainder);

    // Atualiza o comprimento (remove zeros à esquerda)
    size_t new_len = 0;
    while(new_len < working_len && working_str[new_len] == '0') {
      new_len++;
    }
    if(new_len == working_len) {
      break; // Todas as posições são zero
    }
    working_len -= new_len;
    memmove(working_str, working_str + new_len, working_len);
    working_str[working_len] = '\0';

    // Se depois de remover zeros restou apenas um dígito e é zero, paramos
    if(working_len == 1 && working_str[0] == '0') {
      break;
    }
  }

  free(working_str);

  // Se não adicionamos nenhum dígito, adiciona zero
  if(linkedlist_length(bi->digits) == 0) {
    linkedlist_append(bi->digits, 0);
    bi->sign = 1;
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

  int      sign    = num >= 0 ? 1 : -1;
  uint32_t abs_num = (num < 0) ? (uint32_t)(-(int64_t)num) : (uint32_t)num;

  bi->sign = sign;

  // Armazena os valores em ordem little-endian (LSB primeiro)
  // Para números pequenos, apenas um uint32_t é necessário
  linkedlist_append(bi->digits, abs_num);

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

  // Mesmo comprimento, compara valores do MSB para o LSB
  for(int i = (int)a_len - 1; i >= 0; i--) {
    uint32_t a_val = linkedlist_get(a->digits, i);
    uint32_t b_val = linkedlist_get(b->digits, i);

    if(a_val > b_val) {
      return a->sign;
    }
    if(a_val < b_val) {
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
    // Quando os sinais são diferentes, a soma vira uma subtração:
    // a + (-b) = a - |b|  ou  (-a) + b = b - |a|
    if(a->sign == 1) {
      // a é positivo, b é negativo: a + (-b) = a - |b|
      BigInt tmp = *b;
      tmp.sign = 1; // torna b positivo para subtrair
      return bigint_subtract(a, &tmp);
    } else {
      // a é negativo, b é positivo: (-a) + b = b - |a|
      BigInt tmp = *a;
      tmp.sign = 1; // torna a positivo para subtrair
      return bigint_subtract(b, &tmp);
    }
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

  // Soma os valores em base 2^32
  uint64_t carry = 0;
  size_t   i     = 0;

  while(i < max_len || carry > 0) {
    uint64_t sum = carry;

    if(i < a_len) {
      sum += (uint64_t)linkedlist_get(a->digits, i);
    }
    if(i < b_len) {
      sum += (uint64_t)linkedlist_get(b->digits, i);
    }

    linkedlist_append(result->digits, (uint32_t)(sum & UINT32_MAX));
    carry = sum >> 32;
    i++;
  }

  // Remove zeros à esquerda
  linkedlist_remove_leading_zeros(result->digits);

  return result;
}

//funcao do Joao Vitor
//Compara |a| e |b| (apenas magnitude).
//Retorna: 1 se |a| > |b|, 0 se iguais, -1 se |a| < |b|
static int bigint_compare_abs(const BigInt *a, const BigInt *b) {
  size_t a_len = linkedlist_length(a->digits);
  size_t b_len = linkedlist_length(b->digits);

  if(a_len > b_len) return 1;
  if(a_len < b_len) return -1;

  // mesma quantidade de blocos: comparar do MSB -> LSB
  for(long i = (long)a_len - 1; i >= 0; --i) {
    uint32_t av = linkedlist_get(a->digits, (size_t)i);
    uint32_t bv = linkedlist_get(b->digits, (size_t)i);
    if(av > bv) return 1;
    if(av < bv) return -1;
  }
  return 0;
}

BigInt *bigint_multiplicacao(const BigInt *a, const BigInt *b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  size_t a_len = linkedlist_length(a->digits);
  size_t b_len = linkedlist_length(b->digits);

  //Se qualquer operando for zero o codigo retornará 0 com sinal positivo
  if (a_len == 0 || (a_len == 1 && linkedlist_get(a->digits, 0) == 0) || b_len == 0 || (b_len == 1 && linkedlist_get(b->digits, 0) == 0)) {
    BigInt *zero = bigint_create_from_int(0);
    return zero;
  }

  //Cria BigInt resultado com zeros suficientes
  BigInt *resultado = bigint_create_empty(0);
  
  if (resultado == NULL) {
    return NULL;
  }
  
  for (size_t h = 0; h < a_len + b_len; h++) {
    linkedlist_append(resultado->digits, 0);
  }

  //Correção do sinal conforme a regra da multiplicacao
  if (a->sign == b->sign) {
    resultado->sign = +1;
  } else {
    resultado->sign = -1;
  }

  //Loop pra fazer a multiplicacao
  for (size_t i = 0; i < a_len; i++) {
    
    uint64_t ai = (uint64_t) linkedlist_get(a->digits, i);
    uint64_t carry = 0;

    for (size_t j = 0; j < b_len || carry > 0; j++) {
      uint64_t bj;
      if (j < b_len) {
        bj = (uint64_t) linkedlist_get(b->digits, j);
      } else {
        bj = 0;
      }

      uint64_t existing = linkedlist_get(resultado->digits, i + j);
      uint64_t sum = ai * bj + existing + carry;
      linkedlist_set(resultado->digits, i + j, (uint32_t)(sum & UINT32_MAX));

      carry = sum >> 32;
    }
  }

  linkedlist_remove_leading_zeros(resultado->digits);

  //Se o resultado é zero entao o resultado deve ser positivo
  if (linkedlist_length(resultado->digits) == 1 && linkedlist_get(resultado->digits, 0) == 0) {
    resultado->sign = +1;
  }

  return resultado;
}

BigInt *bigint_divisao(const BigInt *a, const BigInt *b) {
  if (!a || !b){
    return NULL;
  }

  //Verifica se é divisão por zero
  size_t b_len = linkedlist_length(b->digits);
  if (b_len == 1 && linkedlist_get(b->digits, 0) == 0) {
    //é divisão por zero
    return NULL;
  }

  //Se a (dividendo) é 0 então o resultado é 0
  size_t a_len = linkedlist_length(a->digits);
  if (a_len == 1 && linkedlist_get(a->digits, 0) == 0) {
    return bigint_create_from_int(0);
  }

  //Calcula o sinal do quociente
  int result_sign;
  if (a->sign == b->sign) {
    result_sign = +1;
  } else {
    result_sign = -1;
  }

  //Faz o calculo somente com valores positivos
  BigInt *dividendo = bigint_copy(a);
  BigInt *divisor = bigint_copy(b);
  dividendo->sign = 1;
  divisor->sign = 1;

  //Prepara o BigInt do quociente inicializando-o com zero
  BigInt *quociente = bigint_create_from_int(0);
  if (!quociente){
    return NULL;
  }

  while (bigint_compare(dividendo, divisor) >= 0) {
  
    //acha maior potência de 2 * divisor <= dividendo
    BigInt *temp = bigint_copy(divisor);
    BigInt *mult = bigint_create_from_int(1);

    while (1) {
      //temp <<= 1
      BigInt *temp2 = bigint_sum(temp, temp);

      if (bigint_compare(temp2, dividendo) > 0) {
        bigint_destroy(temp2);
        break;
      }

      bigint_destroy(temp);
      temp = temp2;

      BigInt *mult2 = bigint_sum(mult, mult);
      bigint_destroy(mult);
      mult = mult2;
    }

    //Subtrai do dividendo
    BigInt *novo_dividendo = bigint_subtract(dividendo, temp);
    bigint_destroy(dividendo);
    dividendo = novo_dividendo;

    //adiciona ao quociente
    BigInt *novo_quociente = bigint_sum(quociente, mult);
    bigint_destroy(quociente);
    quociente = novo_quociente;

    bigint_destroy(temp);
    bigint_destroy(mult);
  }

  //Aplica sinal final
  quociente->sign = result_sign;

  //remove zeros a esquerda
  linkedlist_remove_leading_zeros(quociente->digits);

  //Se zero, então o sinal é positivo
  if (linkedlist_length(quociente->digits) == 1 && linkedlist_get(quociente->digits, 0) == 0) {
    quociente->sign = +1;
  }

  bigint_destroy(dividendo);
  bigint_destroy(divisor);

  return quociente;
}

//funcao do Joao Vitor
//  Subtrai b de a (a - b) e retorna um novo BigInt
BigInt *bigint_subtract(const BigInt *a, const BigInt *b) {
  if(a == NULL || b == NULL) return NULL;

  // casos de sinais
  if(a->sign != b->sign) {
    // a - (-b) = a + b
    BigInt tmp = *b;
    tmp.sign   = -tmp.sign; // inverte o sinal de b
    return bigint_sum(a, &tmp);
  }

  // sinais iguais: resultado tem sinal do maior em | |; subtraímos magnitudes
  int cmp = bigint_compare_abs(a, b);
  if(cmp == 0) {
    // zero
    BigInt *r = bigint_create_empty(0);
    if(!r) return NULL;
    linkedlist_append(r->digits, 0);
    r->sign = 1;
    return r;
  }

  const BigInt *maj = (cmp >= 0) ? a : b; // maior magnitude
  const BigInt *min = (cmp >= 0) ? b : a; // menor magnitude

  BigInt *res = bigint_create_empty(0);
  if(!res) return NULL;

  // se |a| >= |b| sinal = a->sign; caso contrário sinal = -a->sign
  res->sign = (cmp >= 0) ? a->sign : -a->sign;

  uint64_t borrow  = 0;
  size_t   max_len = linkedlist_length(maj->digits);
  size_t   min_len = linkedlist_length(min->digits);

  for(size_t i = 0; i < max_len; ++i) {
    uint64_t av = linkedlist_get(maj->digits, i);
    uint64_t bv = (i < min_len) ? linkedlist_get(min->digits, i) : 0ULL;

    uint64_t subtrahend = bv + borrow;
    uint64_t out;
    if(av >= subtrahend) {
      out    = av - subtrahend;
      borrow = 0;
    } else {
      // empresta 1 bloco de 2^32
      out    = ((1ULL << 32) + av) - subtrahend;
      borrow = 1;
    }
    linkedlist_append(res->digits, (uint32_t)(out & 0xFFFFFFFFu));
  }

  // remove zeros à esquerda (no fim da lista, pois está em LSB-first)
  linkedlist_remove_leading_zeros(res->digits);

  // se virou zero absoluto, normaliza
  if(linkedlist_length(res->digits) == 0) {
    linkedlist_append(res->digits, 0);
    res->sign = 1;
  }

  return res;
}

BigInt *bigint_mod(const BigInt *a, const BigInt *n) {
  if (!a || !n) return NULL;

  //Verifica se tem divisor zero
  size_t n_len = linkedlist_length(n->digits);
  if (n_len == 1 && linkedlist_get(n->digits, 0) == 0) {
    //Erro: mod por zero
    return NULL;
  }

  //Copia os valores
  BigInt *dividendo = bigint_copy(a);
  BigInt *divisor = bigint_copy(n);

  dividendo->sign = 1;
  divisor->sign = 1;

  //q = a/n (quociente obtido pela divisão inteira descartando qualquer fração)
  BigInt *q = bigint_divisao(dividendo, divisor);

  bigint_destroy(dividendo);
  bigint_destroy(divisor);

  if (!q){
    return NULL;
  }
  
  //q*n
  BigInt *multiplica_qn = bigint_multiplicacao(q, n);

  if (!multiplica_qn) {
      bigint_destroy(q);
      return NULL;
  }

  //r = a - q*n
  BigInt *resto = bigint_subtract(a, multiplica_qn);

  bigint_destroy(q);
  bigint_destroy(multiplica_qn);

  if (!resto){
    return NULL;
  }
  
  //Ajusta o resto para ser 0 <= r<|n|
  if (resto->sign == -1) {
    // r = r + n
    BigInt *aj = bigint_sum(resto, n);
    bigint_destroy(resto);
    resto = aj;
  }

  return resto;
}

//Multiplica um array decimal por um multiplicador e adiciona um valor
//Retorna o novo comprimento do array
static size_t multiply_decimal_array_by_uint32_and_add(char *arr, size_t len,
                                                       uint64_t multiplier,
                                                       uint64_t add_value) {
  uint64_t carry = add_value;

  for(size_t i = 0; i < len; i++) {
    carry += (arr[i] - '0') * multiplier;
    arr[i] = (char)(carry % 10) + '0';
    carry /= 10;
  }

  // Adiciona novos dígitos se houver carry
  while(carry > 0) {
    if(len >= 10000) { // Limite de segurança
      break;
    }
    arr[len++] = (char)(carry % 10) + '0';
    carry /= 10;
  }

  return len;
}

// Converte BigInt para string (quem chamar deve liberar o resultado)
char *bigint_to_string(const BigInt *bi) {
  if(bi == NULL) {
    return NULL;
  }

  size_t len = linkedlist_length(bi->digits);

  if(len == 0) {
    char *str = malloc(2);
    if(str == NULL) {
      return NULL;
    }
    str[0] = '0';
    str[1] = '\0';
    return str;
  }

  // Aloca um buffer temporário para construir a string decimal
  // Estimativa: cada uint32_t pode representar até ~10 dígitos decimais
  // Usamos um buffer grande para evitar realocações
  char *decimal_buffer = malloc(10000);
  if(decimal_buffer == NULL) {
    return NULL;
  }

  decimal_buffer[0]  = '0';
  size_t decimal_len = 1;

  // Converte de base 2^32 para decimal
  // Começamos do MSB (mais significativo) para o LSB (menos significativo)
  uint64_t base = ((uint64_t)UINT32_MAX) + 1ULL; // 2^32

  for(int i = (int)len - 1; i >= 0; i--) {
    uint32_t value = linkedlist_get(bi->digits, i);

    // Multiplica o resultado atual por 2^32 e adiciona o próximo valor
    decimal_len = multiply_decimal_array_by_uint32_and_add(
        decimal_buffer, decimal_len, base, value);
  }

  // Calcula o tamanho necessário para a string final (incluindo sinal e
  // terminador)
  size_t str_len = decimal_len + (bi->sign == -1 ? 1 : 0) + 1;
  char  *str     = malloc(str_len);
  if(str == NULL) {
    free(decimal_buffer);
    return NULL;
  }

  int index = 0;

  // Adiciona o sinal se for negativo
  if(bi->sign == -1) {
    str[index++] = '-';
  }

  // Copia os dígitos em ordem reversa (MSB para LSB)
  for(int i = (int)decimal_len - 1; i >= 0; i--) {
    str[index++] = decimal_buffer[i];
  }

  str[index] = '\0';

  free(decimal_buffer);
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


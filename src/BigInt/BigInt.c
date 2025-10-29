#include "BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

// Create a BigInt from a string
BigInt* bigint_create_from_string(const char* str) {
  if (str == NULL) {
    return NULL;
  }

  // Skip leading whitespace
  while (*str == ' ' || *str == '\t') {
    str++;
  }

  // Check for sign
  int sign = 1;
  if (*str == '+') {
    sign = 1;
    str++;
  } else if (*str == '-') {
    sign = -1;
    str++;
  }

  // Skip leading zeros
  while (*str == '0' && *(str + 1) != '\0') {
    str++;
  }

  // Calculate length of digits
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

  // Allocate memory
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

  // Store digits in little-endian order (LSB first)
  for (size_t i = 0; i < len; i++) {
    bi->digits[i] = start[len - 1 - i] - '0';
  }

  // Initialize remaining capacity
  for (size_t i = len; i < bi->capacity; i++) {
    bi->digits[i] = 0;
  }

  return bi;
}

// Create a BigInt from an integer
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

  // Count digits
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

  // Store digits in little-endian order
  for (size_t i = 0; i < len; i++) {
    bi->digits[i] = num % 10;
    num /= 10;
  }

  // Initialize remaining capacity
  for (size_t i = len; i < bi->capacity; i++) {
    bi->digits[i] = 0;
  }

  return bi;
}

// Create an empty BigInt with initial capacity
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

// Destroy a BigInt and free its memory
void bigint_destroy(BigInt* bi) {
  if (bi == NULL) {
    return;
  }

  if (bi->digits != NULL) {
    free(bi->digits);
  }
  free(bi);
}

// Create a copy of a BigInt
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

// Resize the capacity of a BigInt
void bigint_resize(BigInt* bi, size_t new_capacity) {
  if (bi == NULL || new_capacity < bi->length) {
    return;
  }

  char* new_digits = realloc(bi->digits, new_capacity * sizeof(char));
  if (new_digits == NULL) {
    return;
  }

  bi->digits = new_digits;

  // Initialize new capacity to zero
  for (size_t i = bi->capacity; i < new_capacity; i++) {
    bi->digits[i] = 0;
  }

  bi->capacity = new_capacity;
}

// Compare two BigInts
// Returns: 1 if a > b, 0 if a == b, -1 if a < b
int bigint_compare(const BigInt* a, const BigInt* b) {
  if (a == NULL || b == NULL) {
    return 0;
  }

  // Compare signs
  if (a->sign > b->sign) {
    return 1;
  }
  if (a->sign < b->sign) {
    return -1;
  }

  // Signs are equal, compare lengths
  if (a->length > b->length) {
    return a->sign;
  }
  if (a->length < b->length) {
    return -a->sign;
  }

  // Same length, compare digits from MSB to LSB
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

// Add two BigInts and return the result
BigInt* bigint_add(const BigInt* a, const BigInt* b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  // Handle different signs
  if (a->sign != b->sign) {
    // This would require subtraction logic
    // For now, assume both are positive
    // TODO: Implement proper signed addition
  }

  // Ensure we have enough capacity
  size_t max_len = (a->length > b->length) ? a->length : b->length;
  size_t result_capacity = max_len + 10;  // Extra space for carry

  BigInt* result = bigint_create_empty(result_capacity);
  if (result == NULL) {
    return NULL;
  }

  result->sign = a->sign;

  // Add digits
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

  // Remove leading zeros
  while (result->length > 1 && result->digits[result->length - 1] == 0) {
    result->length--;
  }

  return result;
}

// Convert BigInt to string (caller must free the result)
char* bigint_to_string(const BigInt* bi) {
  if (bi == NULL) {
    return NULL;
  }

  // Allocate space for sign and digits plus null terminator
  char* str = malloc((bi->length + 2) * sizeof(char));
  if (str == NULL) {
    return NULL;
  }

  int index = 0;

  // Add sign if negative
  if (bi->sign == -1) {
    str[index++] = '-';
  }

  // Copy digits in reverse order (MSB to LSB)
  for (int i = bi->length - 1; i >= 0; i--) {
    str[index++] = bi->digits[i] + '0';
  }

  str[index] = '\0';

  return str;
}

// Convert BigInt to string and print it
void bigint_print(const BigInt* bi) {
  char* str = bigint_to_string(bi);
  if (str != NULL) {
    printf("%s", str);
    free(str);
  }
}


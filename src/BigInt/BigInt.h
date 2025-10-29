#ifndef BIGINT_H
#define BIGINT_H

#include <stddef.h>

// BigInt structure that stores digits dynamically
typedef struct {
  char* digits;         // Array of digits (0-9) stored as characters
  size_t length;        // Number of digits
  int sign;             // 1 for positive, -1 for negative
  size_t capacity;      // Current capacity of the digits array
} BigInt;

// Create a BigInt from a string
BigInt* bigint_create_from_string(const char* str);

// Create a BigInt from an integer
BigInt* bigint_create_from_int(int num);

// Create an empty BigInt with initial capacity
BigInt* bigint_create_empty(size_t initial_capacity);

// Destroy a BigInt and free its memory
void bigint_destroy(BigInt* bi);

// Create a copy of a BigInt
BigInt* bigint_copy(const BigInt* bi);

// Add two BigInts and return the result
BigInt* bigint_add(const BigInt* a, const BigInt* b);

// Compare two BigInts
// Returns: 1 if a > b, 0 if a == b, -1 if a < b
int bigint_compare(const BigInt* a, const BigInt* b);

// Convert BigInt to string (caller must free the result)
char* bigint_to_string(const BigInt* bi);

// Convert BigInt to string and print it
void bigint_print(const BigInt* bi);

// Resize the capacity of a BigInt
void bigint_resize(BigInt* bi, size_t new_capacity);

#endif // BIGINT_H


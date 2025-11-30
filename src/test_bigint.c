#include "BigInt/BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_case_sum(const char *num1_str, const char *num2_str,
                   const char *expected_str) {
  printf("\n=== Test Case (Sum) ===\n");
  printf("Number 1: %s\n", num1_str);
  printf("Number 2: %s\n", num2_str);

  BigInt *num1 = bigint_create_from_string(num1_str);
  BigInt *num2 = bigint_create_from_string(num2_str);

  if(!num1 || !num2) {
    printf("ERROR: Failed to create BigInt\n");
    if(num1) bigint_destroy(num1);
    if(num2) bigint_destroy(num2);
    return;
  }

  BigInt *result = bigint_sum(num1, num2);

  if(!result) {
    printf("ERROR: Failed to sum BigInts\n");
    bigint_destroy(num1);
    bigint_destroy(num2);
    return;
  }

  printf("Result: ");
  bigint_print(result);
  printf("\n");

  if(expected_str) {
    char *result_str = bigint_to_string(result);
    if(strcmp(result_str, expected_str) == 0) {
      printf("✓ PASSED (matches expected: %s)\n", expected_str);
    } else {
      printf("✗ FAILED (expected: %s, got: %s)\n", expected_str, result_str);
    }
    free(result_str);
  }

  bigint_destroy(num1);
  bigint_destroy(num2);
  bigint_destroy(result);
}

void test_case_subtract(const char *num1_str, const char *num2_str,
                        const char *expected_str) {
  printf("\n=== Test Case (Subtract) ===\n");
  printf("Number 1: %s\n", num1_str);
  printf("Number 2: %s\n", num2_str);

  BigInt *num1 = bigint_create_from_string(num1_str);
  BigInt *num2 = bigint_create_from_string(num2_str);

  if(!num1 || !num2) {
    printf("ERROR: Failed to create BigInt\n");
    if(num1) bigint_destroy(num1);
    if(num2) bigint_destroy(num2);
    return;
  }

  BigInt *result = bigint_subtract(num1, num2);

  if(!result) {
    printf("ERROR: Failed to subtract BigInts\n");
    bigint_destroy(num1);
    bigint_destroy(num2);
    return;
  }

  printf("Result: ");
  bigint_print(result);
  printf("\n");

  if(expected_str) {
    char *result_str = bigint_to_string(result);
    if(strcmp(result_str, expected_str) == 0) {
      printf("✓ PASSED (matches expected: %s)\n", expected_str);
    } else {
      printf("✗ FAILED (expected: %s, got: %s)\n", expected_str, result_str);
    }
    free(result_str);
  }

  bigint_destroy(num1);
  bigint_destroy(num2);
  bigint_destroy(result);
}

int main(void) {
  printf("Testing BigInt Subtract Function\n");
  printf("========================================\n");

  // Test case 1: Basic subtraction (positive - positive, a > b)
  test_case_subtract("100", "50", "50");

  // Test case 2: Basic subtraction (positive - positive, a < b)
  test_case_subtract("50", "100", "-50");

  // Test case 3: Equal numbers (result should be zero)
  test_case_subtract("1234567890123", "1234567890123", "0");

  // Test case 4: Large positive - small positive
  test_case_subtract("12345678901234567890", "9876543210", "12345678891358024680");

  // Test case 5: Small positive - large positive (negative result)
  test_case_subtract("9876543210", "12345678901234567890", "-12345678891358024680");

  // Test case 6: Positive - negative (should be addition)
  test_case_subtract("100", "-50", "150");

  // Test case 7: Negative - positive (should be negative addition)
  test_case_subtract("-100", "50", "-150");

  // Test case 8: Negative - negative (a - (-b) = a + b)
  test_case_subtract("-100", "-50", "-50");

  // Test case 9: Negative - negative (smaller magnitude - larger magnitude)
  test_case_subtract("-50", "-100", "50");

  // Test case 10: Zero - positive
  test_case_subtract("0", "12345678901234567890", "-12345678901234567890");

  // Test case 11: Positive - zero
  test_case_subtract("12345678901234567890", "0", "12345678901234567890");

  // Test case 12: Zero - zero
  test_case_subtract("0", "0", "0");

  // Test case 13: Subtraction with borrow (1000 - 1)
  test_case_subtract("1000", "1", "999");

  // Test case 14: Large subtraction with borrow
  test_case_subtract("100000000000000000000", "1", "99999999999999999999");

  // Test case 15: Multiple borrows
  test_case_subtract("100000000000000000000", "99999999999999999999", "1");

  // Test case 16: 20-digit subtraction
  test_case_subtract("12345678901234567890", "98765432109876543210", "-86419753208641975320");

  // Test case 17: 30-digit subtraction
  test_case_subtract("123456789012345678901234567890", "987654321098765432109876543210", "-864197532086419753208641975320");

  // Test case 18: Different length numbers (longer - shorter)
  test_case_subtract("123456789012345678901234567890", "123", "123456789012345678901234567767");

  // Test case 19: Different length numbers (shorter - longer)
  test_case_subtract("123", "123456789012345678901234567890", "-123456789012345678901234567767");

  // Test case 20: Very large numbers (50 digits)
  test_case_subtract("12345678901234567890123456789012345678901234567890",
                     "98765432109876543210987654321098765432109876543210",
                     "-86419753208641975320864197532086419753208641975320");

  // Test case 21: Subtraction with many borrows
  test_case_subtract("10000000000000000000000000000000000000000000000000", "1",
                     "9999999999999999999999999999999999999999999999999");

  // Test case 22: Negative - zero
  test_case_subtract("-12345678901234567890", "0", "-12345678901234567890");

  // Test case 23: Zero - negative
  test_case_subtract("0", "-12345678901234567890", "12345678901234567890");

  // Test case 24: Large positive - large positive (close values)
  test_case_subtract("999999999999999999999999999999", "999999999999999999999999999998", "1");

  // Test case 25: Single digit subtraction
  test_case_subtract("9", "5", "4");

  // Test case 26: Single digit with negative result
  test_case_subtract("5", "9", "-4");

  printf("\n========================================\n");
  printf("All subtract test cases completed!\n");

  return 0;
}

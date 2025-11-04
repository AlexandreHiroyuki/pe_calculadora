#include "BigInt/BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_case(const char *num1_str, const char *num2_str,
               const char *expected_str) {
  printf("\n=== Test Case ===\n");
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

int main(void) {
  printf("Testing BigInt with numbers > 12 digits\n");
  printf("========================================\n");

  // Test case 1: 13-digit numbers
  test_case("1234567890123", "9876543210987", "11111111101110");

  // Test case 2: 15-digit numbers
  test_case("123456789012345", "987654321098765", "1111111110111110");

  // Test case 3: 20-digit numbers
  test_case("12345678901234567890", "98765432109876543210",
            "111111111011111111100");

  // Test case 4: Different lengths (13 and 15 digits)
  test_case("1234567890123", "987654321098765", "988888888988888");

  // Test case 5: Very large numbers (30 digits)
  test_case("123456789012345678901234567890", "987654321098765432109876543210",
            "1111111110111111111011111111100");

  // Test case 6: Numbers with carry propagation
  test_case("99999999999999999999", "1", "100000000000000000000");

  // Test case 7: Equal large numbers
  test_case("55555555555555555555", "55555555555555555555",
            "111111111111111111110");

  // Test case 8: Zero + large number
  test_case("0", "12345678901234567890", "12345678901234567890");

  // Test case 9: Large number + zero
  test_case("12345678901234567890", "0", "12345678901234567890");

  // Test case 10: 50-digit numbers
  test_case("12345678901234567890123456789012345678901234567890",
            "98765432109876543210987654321098765432109876543210",
            "111111111011111111101111111110111111111011111111100");

  // Test case 11: 100-digit numbers
  test_case("123456789012345678901234567890123456789012345678901234567890123456"
            "7890123456789012345678901234567890",
            "987654321098765432109876543210987654321098765432109876543210987654"
            "3210987654321098765432109876543210",
            "111111111011111111101111111110111111111011111111101111111110111111"
            "11101111111110111111111011111111100");

  // Test case 12: Very large with many carries
  test_case("99999999999999999999999999999999999999999999999999", "1",
            "100000000000000000000000000000000000000000000000000");

  printf("\n========================================\n");
  printf("All test cases completed!\n");

  return 0;
}

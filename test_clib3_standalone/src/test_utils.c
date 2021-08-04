#include "../../clib3/src/utils.h"


#include <CUnit/CUnit.h>

void test_bytes2hex()
{
    char* bytes = "0123456789";
    char string[30];
    bytes2hex((unsigned char*) bytes, 10, string);
    printf("%s\n", string);
}

void test_bytes2hex_unsigned()
{

    unsigned char bytes[] = {0xFF, 0xFE};
    char string[6];
    bytes2hex(bytes, 2, string);
    printf("%s\n", string);
}

void add_utils_tests()
{
    CU_pSuite pSuite;

    pSuite = CU_add_suite("utils", NULL, NULL);
    CU_add_test(pSuite, "test_bytes2hex", test_bytes2hex);
    CU_add_test(pSuite, "test_bytes2hex_unsigned", test_bytes2hex_unsigned);
}

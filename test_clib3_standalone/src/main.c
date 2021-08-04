#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test_linked_list.h"
#include "test_timer.h"
#include "test_tcpmsg.h"

void test_pass(void)
{
    printf(">>> void test_pass(void)\n");
    CU_PASS("Test which will pass\n");
    printf("<<< void test_pass(void)\n");
}

void test_fail(void)
{
    printf(">>> void test_fail(void)\n");
    CU_FAIL("Test which will fail\n");
    printf("<<< void test_fail(void)\n");

}

int main(int argc, char* argv[])
{
    CU_ErrorCode error_code;
    //CU_pSuite pSuite;

    error_code = CU_initialize_registry();
    if (error_code != CUE_SUCCESS)
    {
        fprintf(stderr, "Failed to initialize CUnit registry");
        return 1;
    }

//    pSuite = CU_add_suite("main", NULL, NULL);
//    CU_add_test(pSuite, "test_pass", &test_pass);
//    CU_add_test(pSuite, "test_fail", &test_fail);

//    add_linked_list_tests();
//    add_timer_tests();
    add_tcpmsg_tests();

    CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}

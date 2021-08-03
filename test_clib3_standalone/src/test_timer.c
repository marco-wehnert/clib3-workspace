// Always include own header file to
// keep our own header file clean.
// If you add first other header files
// then the definitions will also be
// visible in the header file.

#include "test_timer.h"
#include "../../clib3/src/timer.h"

// After including our own heade files
// we can now include other header files
#include <CUnit/CUnit.h>
#include <unistd.h> // Needed for function "sleep"

int global_timer_counter = 0;

void timer_callback(timer_vars_t* ptr_timer_vars)
{
    printf("--- void timer_callback(timer_vars_t* ptr_timer_vars)\n");
    global_timer_counter++;
}

void test_timer(void)
{
    timer_vars_t timer_vars;

    printf(">>> void test_timer(void)\n");
    timer_vars.callback = &timer_callback;
    timer_vars.cycle_time = 100000;
    global_timer_counter = 0;
    timer_start(&timer_vars);
    sleep(1);
    timer_stop(&timer_vars);
    CU_ASSERT_EQUAL(global_timer_counter, 10);
    printf("<<< void test_timer(void)\n");
}

void add_timer_tests()
{
    CU_pSuite pSuite;

    pSuite = CU_add_suite("timer", NULL, NULL);
    CU_add_test(pSuite, "test_timer", &test_timer);
}

#include <CUnit/CUnit.h>
#include "../../clib3/src/linked_list.h"
#include <stdlib.h>

void test_ll_push_front(void)
{
    linked_list_t ll;
    ll_init(&ll);
    CU_ASSERT_PTR_NULL(ll.front);
    CU_ASSERT_PTR_NULL(ll.back);
    CU_ASSERT_TRUE(ll.count == 0);
    char* ptr_object = malloc(256);

    ll_push_front(&ll, ptr_object);

    CU_ASSERT_PTR_EQUAL(ptr_object, ll.front->object);
    CU_ASSERT_PTR_EQUAL(ptr_object, ll.back->object);
    ll_clear(&ll);
    free(ptr_object);
}

void test_ll_push_back(void)
{
    linked_list_t ll;
    ll_init(&ll);

    CU_ASSERT_PTR_NULL(ll.front);
    CU_ASSERT_PTR_NULL(ll.back);
    CU_ASSERT_TRUE(ll.count == 0);
    char* ptr_object = malloc(256);

    ll_push_back(&ll, ptr_object);

    CU_ASSERT_PTR_EQUAL(ptr_object, ll.back->object);
    CU_ASSERT_PTR_EQUAL(ptr_object, ll.front->object);
    ll_clear(&ll);
    free(ptr_object);
}

void test_ll_push_pop_front(void)
{
    linked_list_t ll;
    char* ptr;

    ll_init(&ll);
    char* object_1 = malloc(10);
    ll_push_front(&ll, object_1);
    CU_ASSERT_EQUAL(ll.count, 1);

    char* object_2 = malloc(20);
    ll_push_front(&ll, object_2);
    CU_ASSERT_EQUAL(ll.count, 2);

    char* object_3 = malloc(30);
    ll_push_front(&ll, object_3);
    CU_ASSERT_EQUAL(ll.count, 3);

    CU_ASSERT_PTR_EQUAL(ll.front->object, object_3);
    CU_ASSERT_PTR_EQUAL(ll.back->object, object_1);
    CU_ASSERT_PTR_NULL(ll.front->prev);
    CU_ASSERT_PTR_NULL(ll.back->next);

    CU_ASSERT_PTR_EQUAL(ll.front->next->object, object_2);
    CU_ASSERT_PTR_EQUAL(ll.back->prev->object, object_2);

    CU_ASSERT_PTR_EQUAL(ll.front->next->prev, ll.front);
    CU_ASSERT_PTR_EQUAL(ll.front->next->prev->object, object_3);
    CU_ASSERT_PTR_EQUAL(ll.back->prev->next, ll.back);
    CU_ASSERT_PTR_EQUAL(ll.back->prev->next->object, object_1);

    ptr = ll_pop_front(&ll);
    CU_ASSERT_PTR_EQUAL(ptr, object_3);

    ptr = ll_pop_front(&ll);
    CU_ASSERT_PTR_EQUAL(ptr, object_2);

    ptr = ll_pop_front(&ll);
    CU_ASSERT_PTR_EQUAL(ptr, object_1);

    free(object_1);
    free(object_2);
    free(object_3);
}

void test_ll_push_pop_back(void)
{
    linked_list_t ll;
    char* ptr;

    ll_init(&ll);
    char* object_1 = malloc(10);
    ll_push_back(&ll, object_1);
    CU_ASSERT_EQUAL(ll.count, 1);

    char* object_2 = malloc(20);
    ll_push_back(&ll, object_2);
    CU_ASSERT_EQUAL(ll.count, 2);

    char* object_3 = malloc(30);
    ll_push_back(&ll, object_3);
    CU_ASSERT_EQUAL(ll.count, 3);

    CU_ASSERT_PTR_EQUAL(ll.back->object, object_3);
    CU_ASSERT_PTR_EQUAL(ll.front->object, object_1);
    CU_ASSERT_PTR_NULL(ll.back->next);
    CU_ASSERT_PTR_NULL(ll.front->prev);

    CU_ASSERT_PTR_EQUAL(ll.back->prev->object, object_2);
    CU_ASSERT_PTR_EQUAL(ll.front->next->object, object_2);

    CU_ASSERT_PTR_EQUAL(ll.back->prev->next, ll.back);
    CU_ASSERT_PTR_EQUAL(ll.back->prev->next->object, object_3);
    CU_ASSERT_PTR_EQUAL(ll.front->next->prev, ll.front);
    CU_ASSERT_PTR_EQUAL(ll.front->next->prev->object, object_1);

    ptr = ll_pop_back(&ll);
    CU_ASSERT_PTR_EQUAL(ptr, object_3);

    ptr = ll_pop_back(&ll);
    CU_ASSERT_PTR_EQUAL(ptr, object_2);

    ptr = ll_pop_back(&ll);
    CU_ASSERT_PTR_EQUAL(ptr, object_1);

    free(object_1);
    free(object_2);
    free(object_3);
}

void test_ll_remove_element_count_0(void)
{
    linked_list_t ll;
    ll_init(&ll);
    void* object = (void*) 1;

    ll_remove(&ll, object);
    CU_ASSERT_EQUAL(ll.count, 0);
}

void test_ll_remove_element_count_1_not_matching(void)
{
    linked_list_t ll;
    ll_init(&ll);
    void* object_1 = (void*) 1;
    void* object_2 = (void*) 2;

    ll_push_front(&ll, object_1);
    CU_ASSERT_EQUAL(ll.count, 1);
    ll_remove(&ll, object_2);
    CU_ASSERT_EQUAL(ll.count, 1);
    ll_clear(&ll);
}

void test_ll_remove_element_count_1_matching(void)
{
    linked_list_t ll;
    ll_init(&ll);
    void* object_1 = (void*) 1;

    ll_push_front(&ll, object_1);
    CU_ASSERT_EQUAL(ll.count, 1);
    ll_remove(&ll, object_1);
    CU_ASSERT_EQUAL(ll.count, 0);
    CU_ASSERT_PTR_NULL(ll.front);
    CU_ASSERT_PTR_NULL(ll.back);
}

void test_ll_remove_element_count_2_not_matching(void)
{
    linked_list_t ll;
    ll_init(&ll);
    void* object_1 = (void*) 1;
    void* object_2 = (void*) 2;
    void* object_3 = (void*) 3;

    ll_push_back(&ll, object_1);
    ll_push_back(&ll, object_2);

    CU_ASSERT_EQUAL(ll.count, 2);
    ll_remove(&ll, object_3);
    CU_ASSERT_EQUAL(ll.count, 2);
    ll_clear(&ll);
}

void test_ll_remove_element_count_2_matching_front(void)
{
    linked_list_t ll;

    ll_init(&ll);
    void* object_1 = (void*) 1;
    void* object_2 = (void*) 2;

    ll_push_back(&ll, object_1);
    ll_push_back(&ll, object_2);
    CU_ASSERT_EQUAL(ll.front->object, 1);
    CU_ASSERT_EQUAL(ll.back->object, 2);

    ll_remove(&ll, object_1);
    CU_ASSERT_EQUAL(ll.count, 1);
    CU_ASSERT_EQUAL(ll.front->object, object_2);
    ll_clear(&ll);
}

void test_ll_remove_element_count_2_matching_back(void)
{
    linked_list_t ll;

    ll_init(&ll);
    void* object_1 = (void*) 1;
    void* object_2 = (void*) 2;

    ll_push_back(&ll, object_1);
    ll_push_back(&ll, object_2);
    CU_ASSERT_EQUAL(ll.front->object, 1);
    CU_ASSERT_EQUAL(ll.back->object, 2);

    ll_remove(&ll, object_2);
    CU_ASSERT_EQUAL(ll.count, 1);
    CU_ASSERT_EQUAL(ll.front->object, object_1);
    ll_clear(&ll);
}

void test_ll_remove_element_count_3_matching(void)
{
    linked_list_t ll;
    ll_init(&ll);
    void* object_1 = (void*) 1;
    void* object_2 = (void*) 2;
    void* object_3 = (void*) 3;

    ll_push_back(&ll, object_1);
    ll_push_back(&ll, object_2);
    ll_push_back(&ll, object_3);

    ll_remove(&ll, object_2);
    CU_ASSERT_EQUAL(ll.count, 2);
    CU_ASSERT_EQUAL(ll.front->object, object_1);
    CU_ASSERT_EQUAL(ll.back->object, object_3);
    CU_ASSERT_EQUAL(ll.front->next, ll.back);
    CU_ASSERT_EQUAL(ll.back->prev, ll.front);
    ll_clear(&ll);
}

void test_ll_remove_element_count_3_not_matching(void)
{
    linked_list_t ll;
    ll_init(&ll);
    void* object_1 = (void*) 1;
    void* object_2 = (void*) 2;
    void* object_3 = (void*) 3;

    ll_push_back(&ll, object_1);
    ll_push_back(&ll, object_2);
    ll_push_back(&ll, object_3);

    ll_remove(&ll, (void*)5);
    CU_ASSERT_EQUAL(ll.count, 3);
    CU_ASSERT_EQUAL(ll.front->object, object_1);
    CU_ASSERT_EQUAL(ll.back->object, object_3);
    CU_ASSERT_EQUAL(ll.front->next->object, object_2);
    CU_ASSERT_EQUAL(ll.back->prev->object, object_2);
    ll_clear(&ll);

}


void add_linked_list_tests()
{   
    CU_pSuite pSuite;

    pSuite = CU_add_suite("linked_list", NULL, NULL);
    CU_add_test(pSuite, "test_ll_push_front", &test_ll_push_front);
    CU_add_test(pSuite, "test_ll_push_back", &test_ll_push_back);
    CU_add_test(pSuite, "test_ll_push_pop_front", &test_ll_push_pop_front);
    CU_add_test(pSuite, "test_ll_push_pop_back", &test_ll_push_pop_back);
    CU_add_test(pSuite, "test_ll_remove_element_count_0",
            &test_ll_remove_element_count_0);
    CU_add_test(pSuite, "test_ll_remove_element_count_1_matching",
            &test_ll_remove_element_count_1_matching);
    CU_add_test(pSuite, "test_ll_remove_element_count_1_not_matching",
            &test_ll_remove_element_count_1_not_matching);
    CU_add_test(pSuite, "test_ll_remove_element_count_2_not_matching",
            &test_ll_remove_element_count_2_not_matching);
    CU_add_test(pSuite, "test_ll_remove_element_count_2_matching_front",
            &test_ll_remove_element_count_2_matching_front);
    CU_add_test(pSuite, "test_ll_remove_element_count_2_matching_back",
            &test_ll_remove_element_count_2_matching_back);
    CU_add_test(pSuite, "test_ll_remove_element_count_3_matching",
            &test_ll_remove_element_count_3_matching);
    CU_add_test(pSuite, "test_ll_remove_element_count_3_not_matching",
            &test_ll_remove_element_count_3_not_matching);

}
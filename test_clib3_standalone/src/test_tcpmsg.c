#include "test_tcpmsg.h"

#include "../../clib3/src/tcpmsg_client.h"
#include "../../clib3/src/tcpmsg_server.h"
#include "../../clib3/src/linked_list.h"

#include <CUnit/CUnit.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void test_tcpmsg_server_create_shutdown()
{
    int result;
//    tcpmsg_client_vars_t client_vars;
    tcpmsg_server_vars_t server_vars;

    printf(">>> void test_tcpmsg()\n");

//    client_vars.server_ip = "127.0.0.1";
    server_vars.port = 29578;

    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

//    result = TCPMSG_connect_client(&client_vars);
//    sleep(1);

    TCPMSG_shutdown_server(&server_vars);
//    sleep(1);

//    CU_ASSERT_EQUAL(result, 0);

    CU_ASSERT_TRUE(true);
    printf("<<< void test_tcpmsg()\n");

}

void test_tcpmsg_client_connect_disconnect()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_connection_vars_t* connection;
    list_element_t* list_element;
    void* res;


    ll_init(&server_vars.connections);
    server_vars.port = 29578;
    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

    tcpmsg_client_vars_t client_vars;
    client_vars.server_ip = "127.0.0.1";
    client_vars.server_port = 29578;
    result = TCPMSG_connect_client(&client_vars);
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_NOT_EQUAL(client_vars.socket, 0);

    usleep(10000);
    TCPMSG_disconnect_client(&client_vars);
    usleep(10000);

    list_element = server_vars.connections.front;
    while (list_element)
    {
        connection = list_element->object;
        pthread_join(connection->reader_thread_id, &res);
        free(connection);
        list_element = list_element->next;
    }
    ll_clear(&server_vars.connections);


    TCPMSG_shutdown_server(&server_vars);

    usleep(10000);

}

void add_tcpmsg_tests()
{
    CU_pSuite pSuite;

    pSuite = CU_add_suite("tcpmsg", NULL, NULL);
    /*CU_add_test(pSuite,
            "test_tcpmsg_server_create_shutdown",
            &test_tcpmsg_server_create_shutdown);*/
    CU_add_test(pSuite,
            "test_tcpmsg_client_connect_disconnect",
            &test_tcpmsg_client_connect_disconnect);
}



#include "test_tcpmsg.h"

#include "../../clib3/src/tcpmsg_client.h"
#include "../../clib3/src/tcpmsg_server.h"
#include "../../clib3/src/linked_list.h"

#include <CUnit/CUnit.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define ENTER_TEST() printf(">>> %*s\n", -40, __func__)
#define LEAVE_TEST() printf("<<< %*s\n", -40, __func__)

#define LOCALHOST "127.0.0.1"
#define PORT 29578

void test_tcpmsg_server_create_shutdown()
{
    int result;
    tcpmsg_server_vars_t server_vars;

    ENTER_TEST();

    ll_init(&server_vars.connections);
    server_vars.port = PORT;

    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

    TCPMSG_shutdown_server(&server_vars);

    CU_ASSERT_TRUE(true);

    LEAVE_TEST();
}

void test_tcpmsg_client_connect_disconnect()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;

    ENTER_TEST();

    ll_init(&server_vars.connections);
    server_vars.port = PORT;
    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

    result = TCPMSG_connect_client(&client_vars, LOCALHOST, PORT);
    CU_ASSERT_EQUAL(result, 0);

    CU_ASSERT_NOT_EQUAL(client_vars.socket, 0);
    usleep(1000);
    CU_ASSERT_EQUAL(server_vars.connections.count, 1);

    usleep(1000);
    TCPMSG_disconnect_client(&client_vars);
    usleep(1000);


    TCPMSG_shutdown_server(&server_vars);
    CU_ASSERT_EQUAL(server_vars.connections.count, 0);

    usleep(1000);

    LEAVE_TEST();
}

void test_tcpmsg_shutdown_with_connection()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;

    ENTER_TEST();

    ll_init(&server_vars.connections);
    server_vars.port = PORT;
    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

    result = TCPMSG_connect_client(&client_vars, LOCALHOST, PORT);
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_NOT_EQUAL(client_vars.socket, 0);
    usleep(10000);
    CU_ASSERT_EQUAL(server_vars.connections.count, 1);

    TCPMSG_shutdown_server(&server_vars);
    TCPMSG_disconnect_client(&client_vars);

    LEAVE_TEST();
}

void test_tcpmsg_write_client_to_server()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;
    tcpmsg_header_t header;
    uint8_t data[100];

    ENTER_TEST();

    ll_init(&server_vars.connections);
    server_vars.port = PORT;
    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

    result = TCPMSG_connect_client(&client_vars, LOCALHOST, PORT);
    CU_ASSERT_EQUAL(result, 0);

    header.sync_pattern = htons(0x55FF);
    header.msg_type = htons(0x0001);
    header.payload_length = htonl(100);
    write(client_vars.socket, &header, sizeof(header));
    write(client_vars.socket, &data[0], 100);
    usleep(1000);

    TCPMSG_disconnect_client(&client_vars);
    TCPMSG_shutdown_server(&server_vars);

    LEAVE_TEST();
}

void test_tcpmsg_client_connect_to_nowhere()
{
    int result;
    tcpmsg_reader_vars_t client_vars;

    ENTER_TEST();

    memset(&client_vars, 0, sizeof(tcpmsg_reader_vars_t));

    result = TCPMSG_connect_client(&client_vars, LOCALHOST, PORT+1);
    CU_ASSERT_EQUAL(result, -1);

    LEAVE_TEST();
}

void add_tcpmsg_tests()
{
    CU_pSuite pSuite;

    pSuite = CU_add_suite("tcpmsg", NULL, NULL);
    CU_add_test(pSuite,
            "test_tcpmsg_server_create_shutdown",
            &test_tcpmsg_server_create_shutdown);
    CU_add_test(pSuite,
            "test_tcpmsg_client_connect_disconnect",
            &test_tcpmsg_client_connect_disconnect);
    CU_add_test(pSuite,
            "test_tcpmsg_shutdown_with_connection",
            &test_tcpmsg_shutdown_with_connection);
    CU_add_test(pSuite,
            "test_tcpmsg_write_client_to_server",
            &test_tcpmsg_write_client_to_server);
    CU_add_test(pSuite,
            "test_tcpmsg_client_connect_to_nowhere",
            &test_tcpmsg_client_connect_to_nowhere);
}



#include "test_tcpmsg.h"

#include "../../clib3/src/tcpmsg_client.h"
#include "../../clib3/src/tcpmsg_server.h"
#include "../../clib3/src/linked_list.h"
#include "../../clib3/src/utils.h"

#include <CUnit/CUnit.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


#define LOCALHOST "127.0.0.1"
#define PORT 29578


volatile int g_server_callback_counter = 0;
volatile int g_client_callback_counter = 0;

void server_callback(tcpmsg_reader_vars_t* reader, uint8_t* buffer)
{
    ENTER_FUNC();
    g_server_callback_counter++;
    CU_ASSERT_EQUAL(get_sync_pattern((tcpmsg_header_t*)buffer), SYNC_PATTERN);
    CU_ASSERT_EQUAL(get_msg_type((tcpmsg_header_t*)buffer), 1);
    CU_ASSERT_EQUAL(get_payload_length((tcpmsg_header_t*)buffer), 0xFF);
    LEAVE_FUNC();
}

int setup_default_config(
        tcpmsg_server_vars_t* server_vars,
        tcpmsg_reader_vars_t* client_vars)
{
    int ret = 0;
    int result = 0;

    memset(server_vars, 0, sizeof(tcpmsg_server_vars_t));
    memset(client_vars, 0, sizeof(tcpmsg_reader_vars_t));

    server_vars->callback = &server_callback;
    server_vars->port = PORT;
    result = TCPMSG_create_server(server_vars);
    CU_ASSERT_EQUAL(result, 0);

    result = TCPMSG_connect_client(client_vars, LOCALHOST, PORT);
    CU_ASSERT_EQUAL(result, 0);
    usleep(1000);

    return ret;
}

void teardown_default_config(
        tcpmsg_server_vars_t* server_vars,
        tcpmsg_reader_vars_t* client_vars)
{
    TCPMSG_disconnect_client(client_vars);
    TCPMSG_shutdown_server(server_vars);
}

void test_tcpmsg_server_create_shutdown()
{
    int result;
    tcpmsg_server_vars_t server_vars;

    ENTER_FUNC();

    ll_init(&server_vars.connections);
    server_vars.port = PORT;

    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

    TCPMSG_shutdown_server(&server_vars);

    CU_ASSERT_TRUE(true);

    LEAVE_FUNC();
}

void test_tcpmsg_client_connect_disconnect()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;

    ENTER_FUNC();

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

    LEAVE_FUNC();
}

void test_tcpmsg_shutdown_with_connection()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;

    ENTER_FUNC();

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

    LEAVE_FUNC();
}

void test_tcpmsg_write_client_to_server()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;
    tcpmsg_header_t header;
    uint8_t data[100];

    ENTER_FUNC();

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

    LEAVE_FUNC();
}

void test_tcpmsg_client_connect_to_nowhere()
{
    int result;
    tcpmsg_reader_vars_t client_vars;

    ENTER_FUNC();

    memset(&client_vars, 0, sizeof(tcpmsg_reader_vars_t));

    result = TCPMSG_connect_client(&client_vars, LOCALHOST, PORT+1);
    CU_ASSERT_EQUAL(result, -1);

    LEAVE_FUNC();
}

void test_tcpmsg_server_callback()
{
    int result;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;
    uint8_t buffer[0xFF + sizeof(tcpmsg_header_t)];

    ENTER_FUNC();

    g_server_callback_counter = 0;

    memset(&server_vars, 0, sizeof(server_vars));
    server_vars.port = PORT;
    server_vars.callback = &server_callback;
    result = TCPMSG_create_server(&server_vars);
    CU_ASSERT_EQUAL(result, 0);

    memset(&client_vars, 0, sizeof(client_vars));
    result = TCPMSG_connect_client(&client_vars, LOCALHOST, PORT);
    CU_ASSERT_EQUAL(result, 0);

    memset(buffer, 0, sizeof(buffer));
    set_tcpmsg_header((tcpmsg_header_t*)buffer, 1, 0xFF);
    result = write(client_vars.socket, buffer, sizeof(buffer));
    usleep(1000);
    CU_ASSERT_EQUAL(g_server_callback_counter, 1);

    TCPMSG_shutdown_server(&server_vars);
    TCPMSG_disconnect_client(&client_vars);

    LEAVE_FUNC();
}

void test_tcpmsg_send_fragmented_messages()
{
    int result;
    int transmitted = 0;
    int socket;
    int buffer_size = 3 * (0xFF + sizeof(tcpmsg_header_t));
    tcpmsg_header_t* header;
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;
    uint8_t buffer[buffer_size];

    ENTER_FUNC();

    g_server_callback_counter = 0;

    memset(&server_vars, 0, sizeof(server_vars));
    server_vars.port = PORT;
    server_vars.callback = server_callback;
    TCPMSG_create_server(&server_vars);

    memset(&client_vars, 0, sizeof(client_vars));
    TCPMSG_connect_client(&client_vars, LOCALHOST, PORT);

    header = (tcpmsg_header_t*) buffer;
    set_tcpmsg_header(header, 1, 0xFF);
    header = (tcpmsg_header_t*) &buffer[0xFF + sizeof(tcpmsg_header_t)];
    set_tcpmsg_header(header, 1, 0xFF);
    header = (tcpmsg_header_t*) &buffer[2 * (0xFF + sizeof(tcpmsg_header_t))];
    set_tcpmsg_header(header, 1, 0xFF);


    socket = client_vars.socket;

    result = write(socket, buffer, 4); transmitted+=4;
    CU_ASSERT_EQUAL(result, 4);
    usleep(1000);
    CU_ASSERT_EQUAL(g_server_callback_counter, 0);

    result = write(socket, buffer+transmitted, 4); transmitted+=4;
    CU_ASSERT_EQUAL(result, 4);
    usleep(1000);
    CU_ASSERT_EQUAL(g_server_callback_counter, 0);

    // Send one byte more after message of size 0xFF
    result = write(socket, buffer+transmitted, 0x100); transmitted+= 0x100;
    CU_ASSERT_EQUAL(result, 0x100);
    usleep(1000);
    CU_ASSERT_EQUAL(g_server_callback_counter, 1);

    result = write(socket, buffer+transmitted, 0x150); transmitted+= 0x150;
    CU_ASSERT_EQUAL(result, 0x150);
    usleep(1000);
    CU_ASSERT_EQUAL(g_server_callback_counter, 2);

    result = write(socket, buffer+transmitted, buffer_size - transmitted);
    CU_ASSERT_EQUAL(result, buffer_size - transmitted);
    usleep(1000);
    CU_ASSERT_EQUAL(g_server_callback_counter, 3);

    TCPMSG_disconnect_client(&client_vars);
    TCPMSG_shutdown_server(&server_vars);

    LEAVE_FUNC();
}

void test_tcpmsg_server_cleanup_closed_connections()
{
    tcpmsg_server_vars_t server_vars;
    tcpmsg_reader_vars_t client_vars;

    ENTER_FUNC();

    setup_default_config(&server_vars, &client_vars);
    usleep(1000);

    CU_ASSERT_EQUAL(server_vars.connections.count, 1);

    TCPMSG_disconnect_client(&client_vars);
    usleep(1000);
    TCPMSG_connect_client(&client_vars, LOCALHOST, PORT);
    usleep(1000);
    CU_ASSERT_EQUAL(server_vars.connections.count, 1);

    TCPMSG_shutdown_server(&server_vars);
    TCPMSG_disconnect_client(&client_vars);

    LEAVE_FUNC();
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
    CU_add_test(pSuite,
            "test_tcpmsg_server_callback",
            &test_tcpmsg_server_callback);
    CU_add_test(pSuite,
            "test_tcpmsg_send_fragmented_messages",
            &test_tcpmsg_send_fragmented_messages);
    CU_add_test(pSuite,
            "test_tcpmsg_server_cleanup_closed_connections",
            &test_tcpmsg_server_cleanup_closed_connections);
}



#include "tdd_network.h"

static void clear_connection(void **);

connection_manager_t* connection_manager_create()
{
    connection_manager_t* connection_manager = malloc(sizeof(connection_manager_t));

    connection_manager->connections = bag_create(16);

    return connection_manager;
}

void connection_manager_destroy(connection_manager_t* connection_manager)
{
    bag_destroy(connection_manager->connections, clear_connection);
    free(connection_manager);
}

connection_t* connection_manager_new_connection(connection_manager_t* connection_manager, int connection_id)
{
    connection_t* connection = malloc(sizeof(connection_t));

    connection->unique_id = connection_id;
    connection->unreliable_messages_queue = list_create();
    connection->reliable_messages_queue = list_create();

    bag_set_at(connection_manager->connections, connection_id, connection);

    return connection;
}

static void clear_connection(void **data)
{
    connection_t* conn = (connection_t*)*data;

    list_destroy(&conn->unreliable_messages_queue, false, NULL);
    list_destroy(&conn->reliable_messages_queue, false, NULL);

    free(conn);
    conn = NULL;
}
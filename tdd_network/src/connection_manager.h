#ifndef CONNECTION_MANAGER_H_INCLUDED
#define CONNECTION_MANAGER_H_INCLUDED

typedef struct connection
{
    int     unique_id;
    list_t* unreliable_messages_queue;
    list_t* reliable_messages_queue;
} connection_t;

typedef struct connection_manager
{
    bag_t* connections;
} connection_manager_t;

connection_manager_t*   connection_manager_create();
void                    connection_manager_destroy(connection_manager_t*);
connection_t*           connection_manager_new_connection(connection_manager_t*, int);

#endif // CONNECTION_MANAGER_H_INCLUDED
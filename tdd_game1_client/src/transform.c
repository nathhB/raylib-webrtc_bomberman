#include <raymath.h>

#include "transform.h"
#include "vector_utils.h"

transform_node_t* transform_node_create(transform_node_t* parent, Vector2 offset)
{
    transform_node_t* transform_node = malloc(sizeof(transform_node_t));

    transform_node->position = VEC2_ZERO;
    transform_node->offset = offset;
    transform_node->name = NULL;
    transform_node->parent = parent;
    transform_node->children = list_create();
    transform_node->named_children = htable_create();

    return transform_node;
}

void transform_node_destroy(transform_node_t* node)
{
    list_node_t* current_node = node->children->head;

    while (current_node != NULL)
    {
        transform_node_destroy((transform_node_t*)current_node->data);

        current_node = current_node->next;
    }

    list_destroy(&node->children, false, NULL);
    htable_destroy(&node->named_children, false, NULL, false);

    free(node);
}

transform_node_t* transform_node_attach_child(transform_node_t* parent, const char* name, Vector2 offset)
{
    transform_node_t* child_node = transform_node_create(parent, offset);

    list_push_back(parent->children, child_node);

    if (name)
    {
        child_node->name = name;

        htable_add(parent->named_children, name, child_node);
    }

    return child_node;
}

void transform_node_update_hierarchy(transform_node_t* node)
{
    if (node->parent)
    {
        node->position = Vector2Add(node->parent->position, node->offset);
    }

    list_node_t *current_node = node->children->head;

    while (current_node)
    {
        transform_node_update_hierarchy((transform_node_t *)current_node->data);

        current_node = current_node->next;
    }
}
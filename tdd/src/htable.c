#include <stdio.h>
#include <string.h>
#include <math.h>

#include "list.h"
#include "htable.h"

static void             insert_entry(htable_t*, htable_entry_t*);
static void             remove_entry(htable_t*, htable_entry_t*);
static unsigned int     find_free_slot(htable_t*, htable_entry_t*, bool*);
static htable_entry_t*  find_entry(htable_t*, const char*);
static void             grow(htable_t*);
static unsigned long    hash_sdbm(const char*);

htable_t* htable_create()
{
    return htable_create_with_capacity(HTABLE_DEFAULT_INITIAL_CAPACITY);
}

htable_t* htable_create_with_capacity(size_t capacity)
{
    htable_t* htable = malloc(sizeof(htable_t));

    htable->internal_array = malloc(sizeof(htable_entry_t*) * capacity);
    htable->capacity = capacity;
    htable->count = 0;
    htable->load_factor = 0;

    for (size_t i = 0; i < htable->capacity; i++)
    {
        htable->internal_array[i] = NULL;
    }

    return htable;
}

void htable_destroy(htable_t** htable, bool clear_items, htable_item_clear_func_t clear_item_func, bool clear_keys)
{
    for (size_t i = 0; i < (*htable)->capacity; i++)
    {
        htable_entry_t* entry = (*htable)->internal_array[i];

        if (entry)
        {
            if (clear_items)
            {
                clear_item_func(&entry->item);
            }

            if (clear_keys)
            {
                free((void*)entry->key);
            }

            free(entry);
        }
    }

    free((*htable)->internal_array);
    free(*htable);

    *htable = NULL;
}

void htable_add(htable_t* htable, const char* key, void* item)
{
    htable_entry_t* entry = malloc(sizeof(htable_entry_t));

    entry->key = key;
    entry->item = item;

    insert_entry(htable, entry);

    if (htable->load_factor >= HTABLE_LOAD_FACTOR_THRESHOLD)
    {
        grow(htable);
    }
}

void* htable_get(htable_t* htable, const char* key)
{
    htable_entry_t* entry = find_entry(htable, key);

    return entry ? entry->item : NULL;
}

void* htable_remove(htable_t* htable, const char* key)
{
    htable_entry_t* entry = find_entry(htable, key);

    if (entry)
    {
        void* item = entry->item;

        remove_entry(htable, entry);

        return item;
    }

    return NULL;
}

list_t* htable_items(htable_t* htable)
{
    list_t* items = list_create();

    for (int i = 0; i < htable->capacity; i++)
    {
        htable_entry_t* entry = htable->internal_array[i];

        if (entry)
        {
            list_push_back(items, entry->item);
        }
    }

    return items;
}

void htable_print(htable_t* htable)
{
    printf("Entry count: %zu\n", htable->count);
    printf("Entries:\n");

    for (int i = 0; i < htable->capacity; i++)
    {
        htable_entry_t* entry = htable->internal_array[i];

        if (entry)
        {
            printf("\t- %s => ...\n", entry->key);
        }
    }
}

static void insert_entry(htable_t* htable, htable_entry_t* entry)
{
    bool use_existing_slot = false;
    unsigned int slot = find_free_slot(htable, entry, &use_existing_slot);

    entry->slot = slot;
    htable->internal_array[slot] = entry;

    if (!use_existing_slot)
    {
        htable->count++;
        htable->load_factor = (float)htable->count / htable->capacity;
    }
}

static void remove_entry(htable_t* htable, htable_entry_t* entry)
{
    htable->internal_array[entry->slot] = NULL;

    free(entry);

    htable->count--;
    htable->load_factor = htable->count / htable->capacity;
}

static unsigned int find_free_slot(htable_t* htable, htable_entry_t* entry, bool* use_existing_slot)
{
    unsigned long hash = hash_sdbm(entry->key);
    unsigned int slot;

    // quadratic probing

    htable_entry_t* current_entry;
    size_t i = 0;

    do
    {
        slot = (hash + (int)pow(i, 2)) % htable->capacity;
        current_entry = htable->internal_array[slot];

        i++;
    } while (current_entry != NULL && current_entry->key != entry->key);

    if (current_entry != NULL) // this mean the current entry as the same key as the inserted entry
    {
        *use_existing_slot = true;

        free(current_entry);
    }
    
    return slot;
}

static htable_entry_t* find_entry(htable_t* htable, const char* key)
{
    unsigned long hash = hash_sdbm(key);
    unsigned int slot;

    //quadratic probing

    htable_entry_t* current_entry;
    size_t i = 0;

    do
    {
        slot = (hash + (int)pow(i, 2)) % htable->capacity;
        current_entry = htable->internal_array[slot];

        if (current_entry != NULL && strcmp(current_entry->key, key) == 0)
        {
            return current_entry;
        }

        i++;
    } while (i < htable->capacity);
    
    return NULL;
}

static void grow(htable_t* htable)
{
    unsigned int old_capacity = htable->capacity;
    unsigned int new_capacity = old_capacity * 2;
    htable_entry_t** old_internal_array = htable->internal_array;
    htable_entry_t** new_internal_array = malloc(sizeof(htable_entry_t*) * new_capacity);

    for (size_t i = 0; i < new_capacity; i++)
    {
        new_internal_array[i] = NULL;
    }

    htable->internal_array = new_internal_array;
    htable->capacity = new_capacity;
    htable->count = 0;
    htable->load_factor = 0;

    // rehash

    for (size_t i = 0; i < old_capacity; i++)
    {
        if (old_internal_array[i])
        {
            insert_entry(htable, old_internal_array[i]);
        }
    }

    free(old_internal_array);
}

static unsigned long hash_sdbm(const char* str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
#include "hashmap.h"

void init_item_args(item_t *item, size_t k_size, size_t v_size)
{
    item->k = calloc(1, k_size);
    item->v = calloc(1, v_size);
}

void init_items(hashmap_t *map, size_t size)
{
    map->items = (item_t *) calloc(map->size, sizeof(item_t));

    for (size_t i = 0; i < size; ++ i)
    {
        init_item_args(&map->items[i], map->k_size, map->v_size);
    }
}

hashmap_t *init_hashmap(size_t k_size, size_t v_size, hash_function_t hash)
{
    hashmap_t *map = (hashmap_t *) malloc(sizeof(hashmap_t));

    map->k_size = k_size;
    map->v_size = v_size;
    map->count = 0;
    map->size = HASHMAP_INIT_SIZE;
    map->hash = hash;
    init_items(map, map->size);

    return map;
}

void free_item_args(item_t *item)
{
    free(item->k);
    free(item->v);
}

void free_single_item(item_t *item)
{
    free_item_args(item);
    free(item);
}

void free_item(item_t *item)
{
    if (item == NULL)
    {
        return;
    }
    if (item->next != NULL)
    {
        free_item(item->next);
    }
    free_single_item(item);
}

void free_items(item_t* items, size_t size)
{
    for (size_t i = 0; i < size; ++ i)
    {
        free_item(items[i].next);
        free_item_args(&items[i]);
    }
    free(items);
}

void free_hashmap(hashmap_t *map)
{
    free_items(map->items, map->size);
    free(map);
}

void resize_hashmap(hashmap_t *map, size_t new_size)
{
    item_t *item;
    item_t *temp = map->items;
    size_t old_size = map->size;

    map->size = new_size;
    map->count = 0;

    init_items(map, new_size);

    for (size_t b = 0; b < old_size; ++ b)
    {
        item = temp[b].next;

        while (item != NULL)
        {
            hashmap_insert(map, item->k, item->v);

            item = item->next;
        }
    }

    free_items(temp, old_size);
}

void hashmap_insert(hashmap_t *map, const void *k, const void *v)
{
    size_t bucket_index = map->hash(k, map->size);
    item_t *item = &map->items[bucket_index];

    while (item->next != NULL)
    {
        item = item->next;
    }

    item->next = (item_t *) calloc(1, sizeof(item_t));
    item->next->prev = item;

    init_item_args(item->next, map->k_size, map->v_size);
    memcpy(item->next->k, k, map->k_size);
    memcpy(item->next->v, v, map->v_size);

    ++map->count;

    if (map->count > map->size * MAX_LOAD)
    {
        resize_hashmap(map, map->size * 2);
    }
}

STATUS_T hashmap_get(hashmap_t *map, const void *k, void *ret)
{
    size_t bucket_index = map->hash(k, map->size);
    item_t *item = &map->items[bucket_index];

    while (item != NULL)
    {
        if (!memcmp(item->k, k, map->k_size))
        {
            memcpy(ret, item->v, map->v_size);
            return SUCCESS;
        }

        item = item->next;
    }

    return KEY_NOT_FOUND_ERR;
}

void hashmap_delete(hashmap_t *map, const void *k)
{
    size_t bucket_index = map->hash(k, map->size);
    item_t *item = map->items[bucket_index].next;

    while(item != NULL)
    {
        if (!memcmp(item->k, k, map->k_size))
        {
            if (item->next != NULL)
            {
                item->next->prev = item->prev;
            }

            item->prev->next = item->next;

            free_single_item(item);

            -- map->count;

            if (map->count < map->size * MIN_LOAD && map->size > HASHMAP_INIT_SIZE)
            {
                resize_hashmap(map, map->size / 2.);
            }

            return;
        }

        item = item->next;
    }
}

void hashmap_clear(hashmap_t *map)
{
    free_items(map->items, map->size);
    init_items(map, HASHMAP_INIT_SIZE);
    map->size = HASHMAP_INIT_SIZE;
    map->count = 0;
}

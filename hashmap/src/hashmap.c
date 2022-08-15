#include "hashmap.h"

#include <stdio.h>

hashmap_t *init_hashmap(size_t k_size, size_t v_size)
{
    hashmap_t *map = (hashmap_t *) malloc(sizeof(hashmap_t));
    map->k_size = k_size;
    map->v_size = v_size;
    map->count = 0;
    map->size = 8;
    map->items = (item_t *) calloc(map->size, sizeof(item_t));
    for (size_t i = 0; i < map->size; ++ i)
    {
        map->items[i].k = calloc(1, k_size);
        map->items[i].v = calloc(1, v_size);
    }
    return map;
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
    free(item->k);
    free(item->v);
    free(item);
}

void free_hashmap(hashmap_t *map)
{
    for (size_t i = 0; i < map->size; ++ i)
    {
        free_item(map->items[i].next);
        free(map->items[i].k);
        free(map->items[i].v);
    }
    free(map->items);
    free(map);
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

    item->next->k = malloc(map->k_size);
    memcpy(item->next->k, k, map->k_size);
    
    item->next->v = malloc(map->v_size);
    memcpy(item->next->v, v, map->v_size);

    ++map->count;
    if ((double) map->count / (double) map->size > 0.80)
    {
        item_t *temp = map->items;

        map->size *= 2;
        map->count = 0;

        map->items = (item_t *) calloc(map->size, sizeof(item_t));

        for (size_t i = 0; i < map->size; ++ i)
        {
            map->items[i].k = calloc(1, map->k_size);
            map->items[i].v = calloc(1, map->v_size);
        }

        item_t *item;
        for (size_t b = 0; b < map->size / 2; ++ b)
        {
            item = temp[b].next;
            while (item != NULL)
            {
                hashmap_insert(map, item->k, item->v);
                item = item->next;
            }
        }

        for (size_t i = 0; i < map->size / 2; ++ i)
        {
            free_item(temp[i].next);
            free(temp[i].k);
            free(temp[i].v);
        }
        free(temp);
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
    return KEY_NOT_FOUND;
}

void hashmap_delete(hashmap_t *map, const void *k)
{

}

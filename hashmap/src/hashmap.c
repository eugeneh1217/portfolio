#include "hashmap.h"

hashmap_t *init_hashmap(size_t k_size, size_t v_size)
{
    hashmap_t *map = (hashmap_t *) malloc(sizeof(hashmap_t));
    map->k_size = k_size;
    map->v_size = v_size;
    map->count = 0;
    map->items = (item_t *) calloc(1, sizeof(item_t));
    map->items->k = calloc(1, k_size);
    map->items->v = calloc(1, v_size);
    return map;
}

void free_item(item_t *item)
{
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
    item_t *item = map->items;
    free_item(map->items);
    free(map);
}

void hashmap_insert(hashmap_t *map, const void *k, const void *v)
{
    item_t *item = map->items;
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
}

STATUS_T hashmap_get(hashmap_t *map, const void *k, void *ret)
{
    item_t *item = map->items;
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

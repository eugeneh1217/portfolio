#include <stdio.h>

typedef struct items_t
{
    unsigned cnt;
    int *data;
} items_t;

void add_item(items_t *items, int item)
{
    items->data[items->cnt ++] = item;
}

void print_items(items_t *items)
{
    printf("items:\n");
    printf("\tcnt=%u\n", items->cnt);
    printf("\tdata:\n");

    for (int i = 0; i < items->cnt; ++i)
    {
        printf("\t\t%d,\n", items->data[i]);
    }

    printf("\n");
}

int main()
{
    printf("\nDYNAMIC ALLOCATION STARTED\n\n");

    int *first_data = (int *) malloc(sizeof(int));

    int another_int = 0;
    printf("another_int is initially '%d'\n", another_int);

    items_t items = {0, &first_data};
    printf("items initalized\n");
    print_items(&items);

    add_item(&items, 3);
    printf("'3' added to items\n");
    print_items(&items);

    add_item(&items, 2);
    printf("'2' added to items\n");
    print_items(&items);
    printf("another_int remains '%d'\n", another_int);

    printf("\n");

    return 0;
}


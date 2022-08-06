# Memory Allocation in C

## Static Allocation Overwrite in C
`static_allocation` demonstrates overwriting behavior of incremented, statically initialized pointers.
1. A variable is initialized.
2. A pointer to that variable is statically initialized.
3. Another variable is initialized after pointer.
4. Pointer is incremented and value incremented pointer points to is modified.
5. The other variable initialized in step 3. is overwritten with the new value in step 4.

This happens because static initialization allocates memory on the stack. When the pointer is incremented, instead of pointing to fresh memory, it points to the next memory in the stack, which was already used by another variable.

## Dynamic Allocation in C
`dynamic_allocation` demonstrates extendability of dynamically allocated memory.
1. Memory is allocated.
2. Another variable is allocated statically.
3. Elements are added to pointer pointing to dynamically allocated memory.
4. Data of other variable remains intact.

## Dynamic Array in C
`dynamic_array` demonstrates that reallocation is necessary in the event of a large piece of data.
1. `int` pointer, representing array, is dynamically allocated.
2. Value at index 0 of pointer is assigned.
3. If demo is run with `-r` flag, reallocation of correct size occurs.
4. Value at index 100000 is assigned.
5. If reallocation occured, code runs to completion printing the assigned values in the array. Otherwise, code fails with segfault.

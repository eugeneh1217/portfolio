# Demos

A demo can be run with
```bash
bash run_demo.sh <demo_name>`
```

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

This happens because dynamic allocations happens on the heap, which is independent of the stack, and allows memory blocks to grow.

## Dynamic Array in C
`dynamic_array` demonstrates that reallocation is necessary in the event of a large piece of data.
1. `int` pointer, representing array, is dynamically allocated.
2. Value at index 0 of pointer is assigned.
3. If demo is run with `-r` flag, reallocation of correct size occurs.
4. Value at index 100000 is assigned.
5. If reallocation occured, code runs to completion printing the assigned values in the array. Otherwise, code fails with segfault.

Without the reallocation, the other allocation got in the way of the growth of the original memory block. Reallocation moved the array data to a memory block large enough for the new data.

## Struct Size in C
`struct_size` demonstrates memory overhead of storing data in structs. It also demonstrates a technique for storing data without memory overhead by casting and indexing a void pointer.

The overhead of the struct is explained in [this](https://stackoverflow.com/questions/19954818/memory-overhead-for-structs-with-pointers-in-c) stackoverflow post.

This overhead is further explained in [this](https://blog.devgenius.io/c-programming-hacks-01-memory-efficient-struct-design-8e7252c2b986) article as a way to satisfy the requirement that all memory addresses need to be a multiple of the size of the data it holds. For example, ints of size 4 bytes can be stored at memory addresses 4, 8, 12, 16, ... which are multiples of 4, but can't be stored at memory addresses like 2, 6, 9, ... which are not multiples of 4. This article does not explain why this requirement exists.

[This](https://en.wikipedia.org/wiki/Data_structure_alignment) wikipedia article explains that the requirement ensures that all memory for a variable (that is smaller than a word) can be stored and accessed in one memory word. This is called n-byte alignment (smallest measurement of memory is byte here) where n is a power of 2.

[This](https://stackoverflow.com/questions/8752546/how-does-malloc-understand-alignment) stackoverflow post explains that malloc does this alignment with the size that is passed to it.

In conclusion, managing memory with void pointer casts risks multiple cache access to obtain variables smaller than a word. If memory is managed correctly with nice sized variables, this can be avoided. A side-effect of this research is that order of members of a struct matters. Should be from largest to smallest (which is supported by [this](https://jonasdevlieghere.com/order-your-members/#alignmentpadding) article).

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct Arena
{
    /*
    Arena structure.

    Members:
        arena: pointer to the allocated arena.
        occupied: number of bytes in use.
        free: number of bytes available for future reservation by a program.
    */
    char* arena;

    size_t occupied;
    size_t free;

} Arena;

Arena* allocateArena(size_t size)
{
    Arena * arena=(Arena*)malloc(sizeof(Arena));

    if(arena==NULL)
    {
        printf("Memory Allocation Failed");
        exit(EXIT_FAILURE);
    }
    arena->arena=(char*)malloc(size);
    if(arena->arena==NULL)
    {
        printf("Memory pool allocation failed");
        exit(EXIT_FAILURE);
    }
    arena->occupied=0;
    arena->free=size;
    return arena;
    
}

void* myMalloc(Arena* arena, size_t size)
{
    /*
    Reserve a portion of memory from within the arena for the program's use.
    Memory portions  follow alignment rules specified in README.MD.

    Parameters:
        size: size of the memory portion to reserve.
    */
   size_t rules;
   if(size==1)
   {
    rules=1;
   }
   else if((size &(size-1))==0)
   {
    rules=size;
   }
   else if(size<8)
   {
        
        if(size>4)
            {
                rules=4;
            } 
       else if(size>2)
        {
            rules=2;

        }
        else
        {
            rules=1;
        }
 
   }
   else
   {
    rules=8;
   } 
   uintptr_t current_adress=(uintptr_t)(arena->arena+arena->occupied);
   uintptr_t new_adress =((current_adress+rules-1)/rules)*rules;
    size_t padding = new_adress - current_adress;
    if (padding + size > arena->free)
    {
        printf("Not enough space (my alloc)\n");
        exit(EXIT_FAILURE);
    }
   arena->occupied += padding + size;
   arena->free -= padding + size;        //if there are memory leaks check this
    return (void*)new_adress;

}

void* myRealloc(Arena* arena, void* address, size_t old_size, size_t new_size)
{
    /*
    Reallocate the specified memory portion to a new (larger) memory portion of the specified size inside the arena.
    Copy all data to the new memory portion.

    Side note: 
    One disadvantage of this is that we can neither free nor repurpose the initial memory portion due to the simplicity of this arena's
    implementation. Consider the example arena below, where location of data to be reallocated (data_3) is indicated
    by the void pointer (address).

    ARENA: before reallocation
    -------------------------------------------------------------------------------------------------------------------------
    |  padding | data_1 | data_2 |    padding    |     data_3      | padding | data_4 |             free space              |                                                                              |
    ------------------------------------------------------------------------------------------------------------------------
                                                 ^address
    
    ARENA: after reallocation
    ------------------------------------------------------------------------------------------------------------------------
    |  padding | data_1 | data_2 |    padding    |     data_3     | padding | data_4 |        data_3  (reallocated)        |                                                                              |
    -----------------------------------------------------------------------------------------------------------------------
                                                 ^address                             ^new_address

    Once data_3 has been reallocated inside the arena to a memory portion indicated by new_address, the old memory portion, although still
    allocated (and can be accessed if the pointer address is kept preserved) is of no longer use. Most importantly, it cannot be repurposed 
    to be of a different size. You may store data of the same size there, but it's way more convenient to just call my_malloc rather than keeping
    track of the addresses being reallocated.

    Parameters:
        arena: the arena from which memory portions are taken.
        address: the address (of a memory portion inside the arena) which contains data to be reallocated.
        old_size: size of the old memory space indicated by the address.
        new_size: size of the new memory space to be created.
    */
   if(new_size<=old_size)
   {
    return address;
   }
   void* new_adress=realloc(address,new_size);
   if(new_adress==NULL)
   {
    printf("Memory allocation failed realloc\n");
    exit(EXIT_FAILURE);
   }
   arena->occupied=arena->occupied+(new_size-old_size);
   arena->free=arena->free-(new_size-old_size);
   return new_adress;

}

void arenaFree(Arena* arena)
{
    /*
    Free all allocated memory associated with the specified arena.*/
    free(arena->arena);
    free(arena);


    
}

void arenaClear(Arena* arena)
{
    /*
    Clear the arena so that it may be overwritten with new data from the start.

    Parameters:
        arena: arena to be cleared.
    */

   int space = arena->occupied;
   arena->occupied=0;
   arena->free= (arena->free+space);



}

void arenaExpansion(Arena* arena, void** addresses[], size_t sizes[], int num_addresses){
    
    /*Expand the specified arena to be twice the size of the current one.
    Updates all pointers that point to the memory portions inside the current arena
    to now point to the corresponding portions (containing the same data) in the new arena.
    All data is aligned.

    Parameters:
        arena: arena to be expanded
        addresses: array of pointers to pointers of memory portions inside the arena storing data.
        sizes: array of sizes (in bytes) of the memory portions corresponding to the provided addresses.
        num_addresses: total number of addresses
    */
   size_t new_size= 2*(arena ->free+arena->occupied);
   char* new_arena =(char*)malloc(new_size); 
      memcpy(new_arena, arena->arena, arena->occupied);

   for (int i = 0; i < num_addresses; i++) 
    {
        size_t difference = (size_t)(*addresses[i]) - (size_t)(arena->arena);  
        *addresses[i] = (void*)(new_arena+ difference);  
    } 
    
    free(arena->arena);

    arena->arena = new_arena;
    arena->free = new_size - arena->occupied;




}

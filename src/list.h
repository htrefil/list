#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

typedef struct list_t
{
	size_t el_size;
	void* data;
	size_t count;
	size_t capacity;
	size_t prealloc;
} list_t;

/*
	Initialize a new empty list with custom settings.
	It's safe to call list_free() afterwards even if no items were added to it.

	Parameters:
		list - pointer to an uninitialized list
		el_size - size of an element
		prealloc - number of elements to preallocate space for
*/
void list_init_ex(list_t* list, size_t el_size, size_t prealloc);

/*
	Initialize a new empty list with default settings.
	It's safe to call list_free() afterwards even if no items were added to it.

	Parameters:
		list - pointer to an uninitialized list
*/
void list_init(list_t* list, size_t el_size);

/*
	Initialize a new list, copying data and settings from another.

	Parameters:
		list - pointer to an uninitialized list
		other - list to be copied 

	Return:
		1 - success
		0 - no memory
*/
int list_init_copy(list_t* list, const list_t* other);


/*
	Add elements to a list.
	If this operation fails, contents of list remain in the same state as before.

	Parameters:
		list - destination list
		el - pointer to elements
		count - number of elements to be added, a value of 0 has no effect

	Return:
		1 - success
		0 - no memory
*/
int list_add(list_t* list, const void* el, size_t count);

/*
	Remove an element from a list.
	
	Parameters:
		list - list
		index - index of target element, must be smaller than list->count
*/
void list_remove(list_t* list, size_t index);

/*
	Shrink a lists memory to only contain as little memory as needed.

	Parameters:
		list - list

	Return:
		1 - success
		0 - no memory
*/
int list_shrink(list_t* list);

/*
	Move other to list.
	It's safe to call list_free() on other afterwards.

	Parameters:
		list - destination list
		other - other list
*/
void list_move(list_t* list, list_t* other);

/*
	Free a list.
	Has to be called when you're done with a list after calling list_init*().

	Parameters:
		list - list
*/
void list_free(list_t* list);
#endif
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "list.h"

#ifndef LIST_DEFAULT_PREALLOC
#define LIST_DEFAULT_PREALLOC	4
#endif

#define AT_INDEX(ptr, ind, si) ((void*)((uintptr_t)(ptr) + (ind) * (si)))

void list_init_ex(list_t* list, size_t el_size, size_t prealloc)
{
	assert(el_size != 0);

	list->el_size = el_size;
	list->data = NULL;
	list->count = 0;
	list->capacity = 0;
	list->prealloc = prealloc;
}

void list_init(list_t* list, size_t el_size)
{
	list_init_ex(list, el_size, LIST_DEFAULT_PREALLOC);
}

int list_init_copy(list_t* list, const list_t* other)
{
	list->el_size = other->el_size;
	list->count = other->count;
	list->capacity = other->capacity;
	list->prealloc = other->prealloc;

	size_t size = (other->count + other->capacity) * list->el_size;
	if (size == 0)
	{
		list->data = NULL;
		return 1;
	}

	list->data = malloc(size);
	if (list->data == NULL)
		return 0;

	memcpy(list->data, other->data, size);
	return 1;
}

int list_add(list_t* list, const void* el, size_t count)
{
	if (count == 0)
		return 1;

	if (list->capacity >= count)
	{
		list->capacity -= count;

		memcpy(AT_INDEX(list->data, list->count, list->el_size), el, count * list->el_size);

		list->count += count;

		return 1;
	}

	// Looks like we exhausted our capacity.
	void* new_data = malloc((list->count + count + list->prealloc) * list->el_size);
	if (new_data == NULL)
		return 0;

	memcpy(new_data, list->data, list->count * list->el_size);
	memcpy(AT_INDEX(new_data, list->count, list->el_size), el, count * list->el_size);

	free(list->data);
	list->data = new_data;
	list->count += count;
	list->capacity = list->prealloc;

	return 1;
}

void list_remove(list_t* list, size_t index)
{
	assert(index < list->count);

	memmove(AT_INDEX(list->data, index, list->el_size), AT_INDEX(list->data, index + 1, list->el_size), (list->count - index - 1) * list->el_size);
	
	list->capacity++;
	list->count--;
}

int list_shrink(list_t* list)
{
	if (list->capacity <= list->prealloc)
		return 1;

	void* new_data = malloc((list->count + list->prealloc) * list->el_size);
	if (new_data == NULL)
		return 0;

	memcpy(new_data, list->data, list->count * list->el_size);
	
	free(list->data);

	list->data = new_data;
	list->capacity = list->prealloc;

	return 1;
}

void list_move(list_t* list, list_t* other)
{
	list->el_size = other->el_size;
	list->data = other->data;
	list->count = other->count;
	list->capacity = other->capacity;
	list->prealloc = other->prealloc;

	other->el_size = 0;
	other->data = NULL;
	other->count = 0;
	other->capacity = 0;
	other->prealloc = 0;
}

void list_free(list_t* list)
{
	free(list->data);
}
#include <stdlib.h>
#include <assert.h>
#include "Array.h"
#define Array_Initial_Capacity 8

Array* Array_Make(void (*_deleter)(void *))
{
	Array* new = malloc(sizeof(Array));
	assert(new != NULL);

	new->length = 0;
	new->_deleter = _deleter;
	new->_capacity = Array_Initial_Capacity;
	new->elements = calloc(sizeof(void*), new->_capacity);

	return new;
}

void Array_Free(Array* array)
{
	assert(array != NULL);

	if(array->_deleter)
		for(int i = 0 ; i < array->length ; i++ )
			array->_deleter(array->elements[i]);

	free(array->elements);
	free(array);
}

void DoubleCapacity(Array *array)
{
	assert(array != NULL);
	
	void** resized= realloc(array->elements, array->_capacity * 2 * sizeof(void*));
	assert(resized != NULL);
	array->elements = resized;
	array->_capacity *= 2;
}

void Array_Append(Array* array, void *toArray_Append)
{
	assert(array != NULL);
	Array_Insert(array, toArray_Append, array->length);
}

void Array_Insert(Array* array, void *toArray_Insert, size_t index)
{
	assert(array != NULL && toArray_Insert != NULL && index <= array->length);

	size_t newLength = array->length + 1;
	if(newLength > array->_capacity)
		DoubleCapacity(array);

	for(int i = array->length ; i > index ; --i)
		array->elements[i] = array->elements[i - 1];

	array->elements[index] = toArray_Insert;
	++(array->length);
}

void Array_Delete(Array* array, size_t index)
{
	assert(array != NULL && index < array->length);
	
	if(array->_deleter)
		array->_deleter(array->elements[index]);

	for(int i = index ; i < array->length - 1 ; ++i)
		array->elements[i] = array->elements[i + 1];

	--(array->length);
}
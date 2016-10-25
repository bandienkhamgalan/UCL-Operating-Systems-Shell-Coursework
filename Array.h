#ifndef ARRAY_H
#define ARRAY_H

typedef struct 
{
	void** elements;
	size_t length;
	size_t _capacity;
	void (*_deleter)(void *);
} Array;

Array* Array_Make(void (*_deleter)(void *));
void Array_Free(Array* array);
void Array_Append(Array* array, void *toArray_Append);
void Array_Insert(Array* array, void *toArray_Insert, size_t index);
void Array_Delete(Array* array, size_t index);

#endif
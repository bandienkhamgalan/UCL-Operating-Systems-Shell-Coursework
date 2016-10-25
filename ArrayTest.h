#include "greatest/greatest.h"
#include "Array.h"

TEST Array_Make_ReturnsNonNullArray() {
    Array* array = Array_Make(NULL);
    ASSERT(array != NULL);
    Array_Free(array);
    PASS();
}

TEST Array_Make_SetsLengthTo0() {
    Array* array = Array_Make(NULL);
    ASSERT_EQ(array->length, 0);
    Array_Free(array);
    PASS();
}

TEST Array_Append_IncrementsLength() {
    Array* array = Array_Make(NULL);
    ASSERT_EQ(array->length, 0);
    Array_Append(array, "hello");
    ASSERT_EQ(array->length, 1);
    Array_Free(array);
    PASS();
}

TEST Array_Append_AddsElement() {
    Array* array = Array_Make(NULL);
    ASSERT_EQ(array->elements[0], NULL);
    Array_Append(array, "hello");
    ASSERT_STR_EQ(array->elements[0], "hello");
    Array_Free(array);
    PASS();
}

TEST Array_Append_ResizesArrayIfNecessary() {
    Array* array = Array_Make(NULL);
    for(int i = 0 ; i < array->_capacity ; i++)
        Array_Append(array, "hello");
    ASSERT_EQ(array->length, array->_capacity);
    int lengthBeforeArray_Appending = array->length;
    Array_Append(array, "world");
    ASSERT_STR_EQ(array->elements[lengthBeforeArray_Appending], "world");
    ASSERT(array->_capacity > lengthBeforeArray_Appending);
    Array_Free(array);
    PASS();    
}

static int freeFunCalls;
static void* freeFunParam;
void freeFun(void* toFree)
{
    freeFunParam = toFree;
    ++freeFunCalls;
}

TEST Array_Free_CallsFreeFunction() {
    freeFunCalls = 0;
    Array* array = Array_Make(&freeFun);
    for(int i = 0 ; i < 4 ; i++)
        Array_Append(array, "hello");
    ASSERT_EQ(freeFunCalls, 0);
    Array_Free(array);
    ASSERT_EQ(freeFunCalls, 4);
    PASS();    
}

TEST Array_Delete_CallsFreeFunction() {
    freeFunCalls = 0;
    freeFunParam = NULL;
    char *string = "hello";
    Array* array = Array_Make(&freeFun);
    Array_Append(array, string);
    Array_Delete(array, 0);
    ASSERT_EQ(freeFunCalls, 1);
    ASSERT_EQ(freeFunParam, string);
    Array_Free(array);
    PASS();    
}

TEST Array_Delete_DecrementsLength() {
    Array* array = Array_Make(NULL);
    Array_Append(array, "hello");
    ASSERT_EQ(array->length, 1);
    Array_Delete(array, 0);
    ASSERT_EQ(array->length, 0);
    Array_Free(array);
    PASS();    
}

TEST Array_Delete_UpdatesArray() {
    Array* array = Array_Make(NULL);
    Array_Append(array, "hello");
    Array_Append(array, "world");
    Array_Append(array, "yes");
    Array_Append(array, "no");
    Array_Delete(array, 1);
    ASSERT_STR_EQ(array->elements[0], "hello");
    ASSERT_STR_EQ(array->elements[1], "yes");
    ASSERT_STR_EQ(array->elements[2], "no");
    Array_Free(array);
    PASS();    
}

TEST Array_Insert_IncrementsLength() {
    Array* array = Array_Make(NULL);
    Array_Append(array, "hello");
    Array_Append(array, "yes");
    Array_Append(array, "no");
    ASSERT_EQ(array->length, 3);
    Array_Insert(array, "world", 1);
    ASSERT_EQ(array->length, 4);
    Array_Free(array);
    PASS();    
}

TEST Array_Insert_UpdatesArray() {
    Array* array = Array_Make(NULL);
    Array_Append(array, "hello");
    Array_Append(array, "yes");
    Array_Append(array, "no");
    Array_Insert(array, "world", 1);
    ASSERT_STR_EQ(array->elements[0], "hello");
    ASSERT_STR_EQ(array->elements[1], "world");
    ASSERT_STR_EQ(array->elements[2], "yes");
    ASSERT_STR_EQ(array->elements[3], "no");
    Array_Free(array);
    PASS();    
} 

SUITE(ArrayTest)
{
    RUN_TEST(Array_Make_ReturnsNonNullArray);
    RUN_TEST(Array_Make_SetsLengthTo0);

    RUN_TEST(Array_Append_IncrementsLength);
    RUN_TEST(Array_Append_AddsElement);
    RUN_TEST(Array_Append_ResizesArrayIfNecessary);

    RUN_TEST(Array_Insert_IncrementsLength);
    RUN_TEST(Array_Insert_UpdatesArray);

    RUN_TEST(Array_Delete_CallsFreeFunction);
    RUN_TEST(Array_Delete_DecrementsLength);
    RUN_TEST(Array_Delete_UpdatesArray);

    RUN_TEST(Array_Free_CallsFreeFunction);
}
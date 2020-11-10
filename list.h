/**
 * @file
 */

#pragma once

#include <stdlib.h>

typedef long long List_El;

typedef long long List_Iterator;

struct Node
{
    List_El value;
    long long prev;
    long long next;
};

struct List
{
    struct Node *data;
    size_t size;
    size_t capacity;
    long long head;
    long long free;              
    char boost_mode;       
};

typedef enum list_error_en 
{
    LIST_OK = 0,
    LIST_ERROR = 1,
    LIST_OVERFLOW = 2,
    LIST_WRONG_INDEX = 3,
    LIST_WRONG_REALLOC = 4,
    LIST_CYCLE = 6
} List_error;

/*!
Construct list structure
\param lst Pointer to list
\param capacity Capacity of the list in the begining
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListConstruct(struct List *lst, size_t capacity);

/*!
Destruct list structure
\param lst Pointer to list
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListDestruct(struct List *lst);

/*!
Function of dumping list. Creates ListDump.log and ListDump.pdf
\param lst Pointer to list
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListDump(struct List *lst);

/*!
Boosting list. Access from GetIterator will be by O(1). Boost mode turns off if you insert/erase element not in the end of the list. 
Works for O(n)
\param lst Pointer to list
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListBoost(struct List *lst);

/*!
Changes capacity to size. Works ONLY in boosted list
\param lst Pointer to list
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListShrinkToFit(struct List *lst);

/*!
Insert element in the end of the list
\param lst Pointer to list
\param x Element which need to insert in the end
\return Iterator of the element x in list
*/
List_Iterator ListPushBack(struct List *lst, List_El x);

/*!
Insert element in the begin of the list
\param lst Pointer to list
\param x Element which need to insert in the begin
\return Iterator of the element x in list
*/
List_Iterator ListPushFront(struct List *lst, List_El x);

/*!
Erase element in the end of the list
\param lst Pointer to list
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListPopBack(struct List *lst);

/*!
Erase element in front of the list
\param lst Pointer to list
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListPopFront(struct List *lst);

/*!
Check list's size
\param lst Pointer to list
\return Size of the list
*/
size_t ListSize(struct List *lst);

/*!
Check list's boost mode
\param lst Pointer to list
\return 1 if list boosted, 0 if it's not
*/
char isListBoosted(struct List *lst);

/*!
Get iterator by index in list. Works for O(n) if list unboosted, and for O(1) of list boosted
\param lst Pointer to list
\param logic_number Index in list which iterator need to find
\return Iterator by index
*/
List_Iterator GetIterator(struct List *lst, long long logic_number);

/*!
Get iterator by value of element. Finds first entry in list. If there's no element x returns -1
\param lst Pointer to list
\param x Value what need to find 
\return Iterator of the element, or -1 if there's no such value
*/
List_Iterator GetIterator_Value(struct List *lst, List_El x);

/*!
Get value of element in list by iterator
\param lst Pointer to list
\param iter Iterator in list
\return Value of the element or -1 if there's something wrong 
*/
List_El ListGetValue(struct List *lst, List_Iterator iter);

/*!
Get iterator of the first element
\param lst Pointer to list
\return Iterator of the first element
*/
List_Iterator ListBegin(struct List *lst);

/*!
Get iterator of the last element
\param lst Pointer to list
\return Iterator of the last element
*/
List_Iterator ListEnd(struct List *lst);

/*!
Get next iterator by present iterator
\param lst Pointer to list
\param iter Present iterator
\return Next iterator
*/
List_Iterator IteratorIncrease(struct List *lst, List_Iterator iter);

/*!
Get previous iterator by present iterator
\param lst Pointer to list
\param iter Present iterator
\return Previoust iterator
*/
List_Iterator IteratorDecrease(struct List *lst, List_Iterator iter);

/*!
Insert element x before iterator. 
\param lst Pointer to list
\param x Element which need to insert
\param iter Iterator before which need to insert element
\return Iterator of new inserted element
*/
List_Iterator ListInsertBefore(struct List *lst, List_El x, List_Iterator iter);

/*!
Insert element x after iterator
\param lst Pointer to list
\param x Element which need to insert
\param iter Iterator after which need to insert element
\return Iterator of new inserted element
*/
List_Iterator ListInsertAfter(struct List *lst, List_El x, List_Iterator iter);

/*!
Erase element by iterator
\param lst Pointer to list
\param iter Iterator which element need to erase (iterator after that is wrong)
\return Code of verify. LIST_OK if list OK, or another code if there's error
*/
List_error ListErase(struct List *lst, List_Iterator iter);




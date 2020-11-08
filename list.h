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
    LIST_UNDERFLOW = 3,
    LIST_WRONG_INDEX = 4,
    LIST_WRONG_REALLOC = 5,
    LIST_CYCLE = 6
} List_error;

List_error ListConstruct(struct List *lst, size_t capacity);

List_error ListDestruct(struct List *lst);

List_error ListDump(struct List *lst);

List_error ListBoost(struct List *lst);

List_error ListShrinkToFit(struct List *lst);

List_Iterator ListPushBack(struct List *lst, List_El x);

List_Iterator ListPushFront(struct List *lst, List_El x);

List_error ListPopBack(struct List *lst);

List_error ListPopFront(struct List *lst);

List_El ListGetValue(struct List *lst, long long logic_number);

List_error ListInsert(struct List *lst, List_El x, long long logic_number);

List_error ListErase(struct List *lst, long long logic_number);

size_t ListSize(struct List *lst);

char isListBoosted(struct List *lst);

List_El ListGetValueIter(struct List *lst, List_Iterator iter);

List_Iterator ListBegin(struct List *lst);

List_Iterator ListEnd(struct List *lst);

List_Iterator IteratorIncrease(struct List *lst, List_Iterator iter);

List_Iterator IteratorDecrease(struct List *lst, List_Iterator iter);

List_Iterator ListInsertIter(struct List *lst, List_El x, List_Iterator iter);

List_error ListEraseIter(struct List *lst, List_Iterator iter);

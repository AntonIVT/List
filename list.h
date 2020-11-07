#pragma once

typedef long long List_El;

struct Node
{
    List_El value;
    long long prev;
    long long next;
};

struct List
{
    struct Node *data;
    long long size;
    long long  capacity;
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


List_error ListConstruct(struct List *lst, long long capacity);

List_error ListDestruct(struct List *lst);

List_error ListDump(struct List *lst);

List_error ListBoost(struct List *lst);

List_error ListShrinkToFit(struct List *lst);

List_error ListPushBack(struct List *lst, List_El x);

List_error ListPushFront(struct List *lst, List_El x);

List_error ListPopBack(struct List *lst);

List_error ListPopFront(struct List *lst);

List_El ListGetValue(struct List *lst, long long logic_number);

List_error ListInsert(struct List *lst, List_El x, long long logic_number);

List_error ListErase(struct List *lst, long long logic_number);

long long ListSize(struct List *lst);

char isListBoosted(struct List *lst);

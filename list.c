#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

static List_error ListVerify(struct List *lst);

static List_error ListResize(struct List *lst, long long new_cap);

static int FindArrayNumber(struct List *lst, long long logic_number);

static List_error ListInsert_Internal(struct List *lst, List_El x, long long array_number);

static List_error ListErase_Internal(struct List *lst, long long array_number);

#define DEBUG_MODE

List_error ListConstruct(struct List *lst, long long capacity)
{
    assert(lst != NULL);
    
    if (capacity <= 0)
        capacity = 1;
    
    lst->data = (struct Node *)calloc(capacity, sizeof(struct Node));
    
    assert(lst->data != NULL);
    
    lst->free = 0;
    for (int i = 0; i < capacity - 1; i++)
    {
        struct Node free_node = {-1, -1, i + 1};
        lst->data[i] = free_node;
    }
    struct Node last_node = {-1, -1 , -1};
    lst->data[capacity - 1] = last_node;
    
    lst->head = -1;
    lst->capacity = capacity;
    lst->size = 0;
    lst->boost_mode = 0;
    
    return ListVerify(lst);
}

List_error ListDestruct(struct List *lst)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    free(lst->data);
    lst->capacity = 0;
    lst->size =  0;
    lst->head = -1;
    lst->free = -1;
    lst->boost_mode =  0;
    
    return ListVerify(lst);
}

static List_error ListVerify(struct List *lst)
{
    if (lst->data == NULL)
    {
        printf("Ban because lst-data == NULL\n");
        return LIST_ERROR;
    }
    if (lst->size > lst->capacity)
    {
        printf("Ban because of overflow\n");
        return LIST_OVERFLOW;
    }
    if (lst->size < 0)
    {
        printf("Ban because of unferflow\n");
        return LIST_UNDERFLOW;
    }
    
    #ifdef DEBUG_MODE
    if (lst->head != -1)
    {
        int curr_node = lst->data[lst->head].next;
        int logic_number = 1;
        while (curr_node != lst->head && logic_number < lst->size)
        {
            curr_node = lst->data[curr_node].next;
            logic_number++;
        }
        if (logic_number != lst->size)
        {
            printf("There's cycle or something upstream data\n");
            ListDump(lst);
        return LIST_CYCLE;
        }
        curr_node = lst->data[lst->head].prev;
        logic_number = 1;
        while (curr_node != lst->head && logic_number < lst->size)
        {
            curr_node = lst->data[curr_node].prev;
            logic_number++;
        }
        if (logic_number != lst->size)
        {
            printf("There's cycle or something reverse data\n");
            ListDump(lst);
            return LIST_CYCLE;
        }
    }
    if (lst->free != -1)
    {
        int curr_node = lst->free;
        int logic_number = 0;
        while (curr_node != -1 && logic_number < lst->capacity - lst->size)
        {
            curr_node = lst->data[curr_node].next;
            logic_number++;    
        }
        if (logic_number != lst->capacity - lst->size)
        {
            printf("There's cycle or something free data\n");
            ListDump(lst);
            return LIST_CYCLE;
        }
    }
    #endif
    
    return LIST_OK;
}

List_error ListDump(struct List *lst)
{
    assert(lst != NULL);
    
    FILE *dump = fopen("ListDump.log", "wb");
    fprintf(dump, "================== LIST DUMP ==================\n"
            "Size       = %i\n"
            "Capacity   = %i\n"
            "Head       = %i\n"
            "Free       = %i\n"
            "Boost Mode = %i\n",
            lst->size, lst->capacity, lst->head, lst->free, lst->boost_mode);
    fprintf(dump,     "\nDATA:\n"
                      "№    |     value     | next | prev |\n");
    for (int i = 0; i < lst->capacity; i++)
    {
        fprintf(dump, "[%2i] |%14lli |  %2i  |  %2i  |\n", i, lst->data[i].value, lst->data[i].next, lst->data[i].prev);
    }
    fprintf(dump, "================== THE END ====================\n");
    
    fclose(dump);
    
    FILE *graph = fopen("graph.gv", "wb");
    
    fprintf(graph, "digraph structs {\n"
                   "    node [shape=record];\n");
    
    const char * color = NULL;   
    for (int i = 0; i < lst->capacity; i++)
    {   
        if (lst->data[i].prev != -1)
            color = "#6659E1";
        else
            color = "#1CAC1C";
        
        fprintf(graph," struct_%i [shape=record, style = filled, fillcolor = \"%s\", "
                      "label=\" value = %i | number_array = %i |"
                      " {prev = %i | next = %i} \"];\n",
                      i, color, lst->data[i].value, i, lst->data[i].prev, lst->data[i].next);
    }               
    
    if (lst->size >= 1)
    {
        fprintf(graph, "struct_%i [style = filled, fillcolor = \"#E5EF5A\"];\n", lst->head);
        if (lst->size >= 2)
            fprintf(graph, "struct_%i [style = filled, fillcolor = \"#FF353C\"];\n", lst->data[lst->head].prev);
    }
    
    for (int i = 0; i < lst->capacity; i++)
    {
        if (lst->data[i].prev == -1)
        {
            if (lst->data[i].next != -1)
                fprintf(graph, "struct_%i -> struct_%i;\n", i, lst->data[i].next);
        }
        else
        {
            fprintf(graph, "struct_%i -> struct_%i;\n", i, lst->data[i].next);
            fprintf(graph, "struct_%i -> struct_%i;\n", i, lst->data[i].prev);
        }
    }
    
    fprintf(graph, "}\n");
    
    fclose(graph);
    
    system("circo -Tpdf graph.gv -o ListDump.pdf && rm graph.gv");
    
    return LIST_OK;
}

static List_error ListResize(struct List *lst, long long new_cap)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (new_cap < lst->size)
        return LIST_ERROR;
    
    struct Node *new_data = (struct Node *)realloc(lst->data, new_cap * sizeof(struct Node));
    
    if (new_data == NULL)
        return LIST_WRONG_REALLOC;
    
    lst->data = new_data;
    
    if (new_cap == lst->size)
    {
        lst->capacity = new_cap;
        return LIST_OK;
    }
    
    int last_free = lst->free;
    if (last_free == -1)
        lst->free = lst->capacity;
    else    
    {
        while(lst->data[last_free].next != -1)
            last_free = lst->data[last_free].next;
        
        lst->data[last_free].next = lst->capacity;
    }
    for (int i = lst->capacity; i < new_cap - 1; i++)
    {
        struct Node free_node = {-1, -1, i + 1};
        lst->data[i] = free_node;
    }
    struct Node last_node = {-1, -1, -1};
    lst->data[new_cap - 1] = last_node;
        
    lst->capacity = new_cap;
    return LIST_OK;
}

List_error ListBoost(struct List *lst)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (lst->boost_mode == 1)
        return LIST_OK;
    
    if (lst->size == 0)
    {
        lst->boost_mode = 1;
        return LIST_OK;
    }
    
    struct Node *new_data = (struct Node *)calloc(lst->capacity, sizeof(struct Node));
    
    if (new_data == NULL)
        return LIST_WRONG_REALLOC;
    
    int curr_node = lst->head;
    for (int i = 0; i < lst->size; i++)
    {
        new_data[i].value = lst->data[curr_node].value;
        new_data[i].next = (i + 1) % lst->size;
        new_data[i].prev = (i - 1 + lst->size) % lst->size;
        curr_node = lst->data[curr_node].next;
    }
    
    free(lst->data);
    lst->data = new_data;
    lst->capacity = lst->size;
    lst->free = -1;
    lst->head = 0;
    lst->boost_mode = 1;
        
    return LIST_OK;
}

List_error ListShrinkToFit(struct List *lst)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (lst->boost_mode == 1)
        return ListResize(lst, lst->size);
    else
        return LIST_ERROR;
}

static int FindArrayNumber(struct List *lst, long long logic_number)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return -1;
    
    if (logic_number >= lst->size || logic_number < 0)
        return -1;
    
    int array_number = lst->head; 
    
    for (int i = 0; i < logic_number; i++)
        array_number = lst->data[array_number].next;
    
    return array_number;
}

static List_error ListInsert_Internal(struct List *lst, List_El x, long long array_number)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (array_number < 0 || array_number >= lst->capacity)       
        return LIST_WRONG_INDEX;
    
    if (lst->data[array_number].prev == -1 && !(lst->size == 0 && array_number == 0))
        return LIST_WRONG_INDEX;
    
    if (lst->capacity == lst->size)
    {
        if (ListResize(lst, lst->capacity * 2) != LIST_OK)
            return LIST_WRONG_REALLOC;
    }
    
    int free_tmp = lst->free;
    lst->free = lst->data[free_tmp].next;                    
    
    if (lst->size == 0)
    {   
        lst->head = free_tmp;
        struct Node new_node = {x, lst->head, lst->head};
        lst->data[lst->head] = new_node;
        
        lst->size++;
        return LIST_OK;
    }
    
    struct Node new_node = {x, lst->data[array_number].prev, array_number};
    
    lst->data[free_tmp] = new_node;
    lst->data[new_node.prev].next = free_tmp;
    lst->data[new_node.next].prev = free_tmp;
        
    lst->size++;
    
    return LIST_OK;
}

static List_error ListErase_Internal(struct List *lst, long long array_number)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (array_number < 0 || array_number >= lst->capacity) 
        return LIST_ERROR;
    
    if (lst->data[array_number].prev == -1)
        return LIST_ERROR;
    
    if (lst->size == 1)
    {
        lst->head = -1;
        
        struct Node dead_node = {-1, -1, lst->free};
        lst->data[array_number] = dead_node;
        lst->free = array_number;
        
        lst->size--;
        lst->boost_mode = 0;
        
        return LIST_OK;
    }
    
    if (lst->head == array_number)
        lst->head = lst->data[array_number].next;
    
    lst->data[lst->data[array_number].prev].next = lst->data[array_number].next;
    lst->data[lst->data[array_number].next].prev = lst->data[array_number].prev;
    
    struct Node dead_node = {-1, -1, lst->free};
    lst->data[array_number] = dead_node;
    lst->free = array_number;
    
    lst->size--;
    
    return LIST_OK;
}

List_error ListPushBack(struct List *lst, List_El x)
{
    assert(lst != NULL);
        
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (lst->size > 0)
        return ListInsert_Internal(lst, x, lst->head);
    else
        return ListInsert_Internal(lst, x, 0);
}

List_error ListPushFront(struct List *lst, List_El x)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    List_error check = LIST_OK;

    if (lst->boost_mode == 1) lst->boost_mode = 0;
    
    if (lst->size > 0)
        check = ListInsert_Internal(lst, x, lst->head);
    else
        check = ListInsert_Internal(lst, x, 0);
    
    if (check == LIST_OK)
        lst->head = lst->data[lst->head].prev;
    
    return check;
}

List_error ListPopBack(struct List *lst)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (lst->size == 0)
        return LIST_ERROR;
    
    return ListErase_Internal(lst, lst->data[lst->head].prev);    
}

List_error ListPopFront(struct List *lst)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (lst->size == 0)
        return LIST_ERROR;
    
    if (lst->boost_mode == 1) lst->boost_mode = 0;
    
    return ListErase_Internal(lst, lst->head);    
}

List_El ListGetValue(struct List *lst, long long logic_number)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return -1;
        
    if (lst->boost_mode == 0)
        return lst->data[FindArrayNumber(lst, logic_number)].value;
    else
        return lst->data[logic_number].value;    
}

List_error ListInsert(struct List *lst, List_El x, long long logic_number)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
        
    if (logic_number > lst->size || logic_number < 0)
        return LIST_WRONG_INDEX;
        
    if (logic_number == lst->size)
        return ListPushBack(lst, x);
    
    lst->boost_mode = 0;
    
    if (logic_number == 0)
        return ListPushFront(lst, x);
    
    return ListInsert_Internal(lst, x, FindArrayNumber(lst, logic_number));
}

List_error ListErase(struct List *lst, long long logic_number)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return ListVerify(lst);
    
    if (logic_number >= lst->size || logic_number < 0)
        return LIST_WRONG_INDEX;
    
    if (logic_number == lst->size - 1)
        return ListPopBack(lst);
    
    lst->boost_mode = 0;
    
    if (logic_number == 0)
        return ListPopFront(lst);
    
    return ListErase_Internal(lst, FindArrayNumber(lst, logic_number));
}

long long ListSize(struct List *lst)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return -1;
    
    return lst->size;
}

char isListBoosted(struct List *lst)
{
    assert(lst != NULL);
    
    if (ListVerify(lst) != LIST_OK)
        return -1;
    
    return lst->boost_mode;
}






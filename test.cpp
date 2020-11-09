#include "list.h"
#include <stdio.h>
#include <gtest/gtest.h>

#define List(cond) ListGetValue(&list, cond)

TEST(construct_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 10);
    
    ASSERT_EQ(LIST_OK, ListDump(&list));
    
    ListDestruct(&list);
}

TEST(push_back_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 10);
    
    ListPushBack(&list, 1);
    List_Iterator iterator = ListPushBack(&list, 2);
    ListPushBack(&list, 3);
    
    ListDump(&list);
    
    ASSERT_EQ(2, List(iterator));
    ASSERT_EQ(1, List(IteratorDecrease(&list, iterator)));
    ASSERT_EQ(3, List(IteratorIncrease(&list, iterator)));
    
    ListDestruct(&list);
}

TEST(push_front_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 20; i++)
        ListPushBack(&list, (i * 10));
    
    ListPushFront(&list, 5000);
    List_Iterator iterator = ListPushFront(&list, 6000);
    
    ListDump(&list);
    
    ASSERT_EQ(5000, List(IteratorIncrease(&list, iterator)));
    ASSERT_EQ(6000, List(iterator));
    ASSERT_EQ(190, List(ListEnd(&list)));
    
    ListDestruct(&list);
}

TEST(resize_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 1);
    
    for (int i = 0; i < 200; i++)
        ListPushBack(&list, (i * 10));
    
    ASSERT_EQ(LIST_OK, ListDump(&list));
    
    ListDestruct(&list);
}

TEST(insert_before, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    List_Iterator iterator = ListPushBack(&list, 1); 
    
    for (int i = 2; i <= 100; i++)
        iterator = ListInsertBefore(&list, i, iterator);
    
    ListDump(&list);
    
    ASSERT_EQ(100, ListGetValue(&list, ListBegin(&list)));
    ASSERT_EQ(1, ListGetValue(&list, ListEnd(&list)));
    
    iterator = GetIterator(&list, 50);
    iterator = ListInsertBefore(&list, 5000, iterator);
    
    ASSERT_EQ(5000, ListGetValue(&list, iterator));
    ASSERT_EQ(51, ListGetValue(&list, IteratorDecrease(&list, iterator)));
    ASSERT_EQ(50, ListGetValue(&list, IteratorIncrease(&list, iterator)));
    
    ListDestruct(&list);
}

TEST(insert_after, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++)
        ListPushBack(&list, i);
    
    ListInsertAfter(&list, 5000, ListEnd(&list));
    ListInsertAfter(&list, 1000, ListBegin(&list));
    
    ASSERT_EQ(5000, List(ListEnd(&list)));
    ASSERT_EQ(0, List(ListBegin(&list)));
    ASSERT_EQ(1000, List(IteratorIncrease(&list, ListBegin(&list))));
    
    ListDestruct(&list);
}

TEST(pop_front_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++) 
        ListPushBack(&list, i * 2);
    
    ListPopFront(&list);
    ListPopFront(&list);
    
    ListDump(&list);
    
    ASSERT_EQ(4, List(ListBegin(&list)));
    ASSERT_EQ(6, List(IteratorIncrease(&list, ListBegin(&list))));
    ASSERT_EQ(198, List(ListEnd(&list)));
    
    ListDestruct(&list);
}

TEST(pop_back_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++) 
        ListPushBack(&list, i * 2);
        
    ListPopBack(&list);
    ListPopBack(&list);
    
    ListDump(&list);
    
    ASSERT_EQ(194, List(ListEnd(&list)));
    ASSERT_EQ(0, List(ListBegin(&list)));
    
    ListDestruct(&list);
}

TEST(boost_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 10000; i++)
        ListPushBack(&list, i);
    
    List_Iterator iterator = GetIterator(&list, 1999);
    
    for (int i = 0; i < 1000; i++)
        ListErase(&list, IteratorIncrease(&list, iterator));
        
    ListBoost(&list);
    
    for (int i = 2000; i < 5000; i++)
    {
        ASSERT_TRUE(isListBoosted(&list));
        ASSERT_EQ(i + 1000, List(GetIterator(&list, i)));
    }
    
    ListDestruct(&list);
}

TEST(shrink_to_fit_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 200; i++)
        ListPushBack(&list, i);
    
    ListBoost(&list);
    ListShrinkToFit(&list);
    
    ListDump(&list);
    
    for (int i = 0; i < 200; i++)
    {   
        ASSERT_TRUE(isListBoosted(&list));
        ASSERT_EQ(i, List(GetIterator(&list, i))); 
    }
    
    ListDestruct(&list);
}

TEST(iterator_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++)
        ListPushBack(&list, i);
    
    List_Iterator iterator =  ListPushBack(&list, 1000);
        
    ListDump(&list);    
        
    ASSERT_EQ(1000, List(iterator));
    iterator = IteratorDecrease(&list, iterator);
    
    for (int i = 99; iterator != ListEnd(&list); i--)
    {
        ASSERT_EQ(i,  List(iterator));
        iterator = IteratorDecrease(&list, iterator);
    }
    
    ListDump(&list);
    ListDestruct(&list);
}

TEST(iterator_test, test2)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++)
        ListPushBack(&list, i);
    
    List_Iterator iterator = ListBegin(&list);
    
    for (int i = 0; i < 50; i++)
        iterator = IteratorIncrease(&list, iterator);
    
    ListDump(&list);
    
    for (int i = 50; i < 100; i++)
    {
        ASSERT_EQ(i, List(iterator));
        iterator = IteratorIncrease(&list, iterator);
    }

    ListDestruct(&list);
}

TEST(iterator_test, test3)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++)
        ListPushBack(&list, i);
    
    for (int i = 20; i < 40; i++)
        ListErase(&list, GetIterator(&list, 20));
        
    ListBoost(&list);
    List_Iterator iterator = ListBegin(&list);
    
    for (int i = 0; i < 20; i++)
        iterator = IteratorIncrease(&list, iterator);
    
    ASSERT_EQ(20, iterator);
    
    ListDump(&list);    
    
    for (int i = 0; i < 20; i++)
    {
        ASSERT_EQ(i + 40, List(iterator));
        iterator = IteratorIncrease(&list, iterator);
    }
    ListDump(&list);
    ListDestruct(&list);
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();  
}
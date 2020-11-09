#include "list.h"
#include <stdio.h>
#include <gtest/gtest.h>

#define List(cond) ListGetValue(&list, cond)
#define ListIt(iter) ListGetValueIter(&list, iter)

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
    ListPushBack(&list, 2);
    ListPushBack(&list, 3);
    
    ListDump(&list);
    
    ASSERT_EQ(2, List(1));
    ASSERT_EQ(1, List(0));
    ASSERT_EQ(3, List(2));
    
    ListDestruct(&list);
}

TEST(push_front_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 20; i++)
        ListPushBack(&list, (i * 10));
    
    ListPushFront(&list, 5000);
    ListPushFront(&list, 6000);
    
    ListDump(&list);
    
    ASSERT_EQ(5000, List(1));
    ASSERT_EQ(6000, List(0));
    ASSERT_EQ(180, List(20));
    
    ListDestruct(&list);
}

TEST(resize_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 20; i++)
        ListPushBack(&list, (i * 10));
    
    ListDump(&list);
    
    ASSERT_EQ(50, List(5));
    ASSERT_EQ(20, List(2));
    ASSERT_EQ(180, List(18));
    
    ListDestruct(&list);
}

TEST(insert_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 20; i++)
        ListPushBack(&list, (i * 10));
    
    ListInsert(&list, 5000, 9);
    ListInsert(&list, 6000, 0);
    
    ListDump(&list);
    
    ASSERT_EQ(5000, List(10));
    ASSERT_EQ(6000, List(0));
    ASSERT_EQ(190 , List(21));
    ASSERT_EQ(90  , List(11));
    ASSERT_EQ(0   , List(1));
    
    ListDestruct(&list);
}

TEST(insert_test, tes2)
{
    struct List list = {0};
    ListConstruct(&list, 0);
    
    ListBoost(&list);
    
    ListPushBack(&list, 1);
    ASSERT_EQ(LIST_OK, ListPopFront(&list));
    
    List_Iterator iterator = ListPushFront(&list, 2);
    ASSERT_EQ(2, ListIt(iterator));
    ASSERT_EQ(2, List(0));
    
    ASSERT_EQ(LIST_OK, ListEraseIter(&list, iterator));
           
    ListDump(&list);
    
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
    
    ASSERT_EQ(4, List(0));
    ASSERT_EQ(198, List(97));
    
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
    
    ASSERT_EQ(194, List(97));
    ASSERT_EQ(0, List(0));
    
    ListDestruct(&list);
}

TEST(erase_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++) 
        ListPushBack(&list, i * 2);
        
    ListErase(&list, 1);
    ListErase(&list, 10);
    
    ListDump(&list);
    
    ASSERT_EQ(0, List(0));
    ASSERT_EQ(4, List(1));
    ASSERT_EQ(20, List(9));
    ASSERT_EQ(24, List(10));
    ASSERT_EQ(26, List(11));
    ASSERT_EQ(198, List(97));
    
    ListDestruct(&list);
}

TEST(boost_test, test1)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 10000; i++)
        ListPushBack(&list, i);
    
    ListBoost(&list);
    
    for (int i = 2500; i < 5000; i++)
        ASSERT_EQ(i, List(i) );
    
    ListDestruct(&list);
}

TEST(boost_test, test2)
{
    struct List list = {0};
    ListConstruct(&list, 2);
    
    for (int i = 0; i < 100; i++)
        ListPushBack(&list, i);
        
    for (int i = 0; i < 20; i++)
        ListErase(&list, 50);
    
    ListBoost(&list);
    ListDump(&list);
    
    for (int i = 50; i < 70; i++)
    {
        ASSERT_EQ(i + 20, List(i));
        ASSERT_EQ(1, isListBoosted(&list));
    }
    
    for (int i = 0; i < 1000; i++)
    {
        ListPushBack(&list, i);
        ASSERT_EQ(1, isListBoosted(&list));
    }
    
    for (int i = 0; i < 1000; i++)
    {
        ListPopBack(&list);
        ASSERT_EQ(1, isListBoosted(&list));
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
        ASSERT_EQ(i, List(i) ); 
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
        
    ASSERT_EQ(1000, ListIt(iterator));
    iterator = IteratorDecrease(&list, iterator);
    
    for (int i = 99; iterator != ListEnd(&list); i--)
    {
        ASSERT_EQ(i,  ListIt(iterator));
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
        ASSERT_EQ(i, ListIt(iterator));
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
        ListErase(&list, 20);
        
    ListBoost(&list);
    List_Iterator iterator = ListBegin(&list);
    
    for (int i = 0; i < 20; i++)
        iterator = IteratorIncrease(&list, iterator);
    
    ASSERT_EQ(20, iterator);
    
    ListDump(&list);    
    
    for (int i = 0; i < 20; i++)
    {
        ASSERT_EQ(i + 40, ListGetValueIter(&list, iterator));
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
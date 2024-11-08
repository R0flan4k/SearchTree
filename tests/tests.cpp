#include "search_tree.h"
#include "gtest/gtest.h"

#include <unordered_map>
#include <vector>

using It = typename RangeQueries::search_tree_t<int>::NodeIt;

TEST(SearchTree, BalanceInsert)
{
    RangeQueries::search_tree_t<int> st{}, st1{}, st2{};
    It n1 = st.insert(10), n2 = st.insert(15), n3 = st.insert(20),
       n4 = st1.insert(15), n5 = st1.insert(4), n6 = st1.insert(5);

    std::vector<int> input2{15, 4, 5, 13, 2, 18, 14, 10, 7};
    for (auto i : input2)
        st2.insert(i);

    EXPECT_EQ(n1->parent(), n2);
    EXPECT_EQ(n1->left(), st.nil());
    EXPECT_EQ(n1->right(), st.nil());
    EXPECT_EQ(n2->parent(), st.nil());
    EXPECT_EQ(n2->left(), n1);
    EXPECT_EQ(n2->right(), n3);
    EXPECT_EQ(n3->parent(), n2);
    EXPECT_EQ(n3->right(), st.nil());
    EXPECT_EQ(n3->left(), st.nil());

    EXPECT_EQ(n4->parent(), n6);
    EXPECT_EQ(n4->right(), st1.nil());
    EXPECT_EQ(n4->left(), st1.nil());
    EXPECT_EQ(n5->parent(), n6);
    EXPECT_EQ(n5->right(), st1.nil());
    EXPECT_EQ(n5->left(), st1.nil());
    EXPECT_EQ(n6->parent(), st1.nil());
    EXPECT_EQ(n6->right(), n4);
    EXPECT_EQ(n6->left(), n5);
}

TEST(SearchTree, Bounds)
{
    RangeQueries::search_tree_t<int> st{}, st1{}, st2{};
    std::vector<int> input{15, 4, 5, 13, 2, 18, 14, 10, 7}, input1{10, 20},
        input2{0, 44, 45, 1, 4, 5, 49, 57, 61, 72, 84, 99, 21, 25, 36, 46};
    std::unordered_map<int, It> nodes, nodes1, nodes2;
    for (auto i : input)
        nodes.emplace(i, st.insert(i));
    for (auto i : input1)
        nodes1.emplace(i, st1.insert(i));
    for (auto i : input2)
        nodes2.emplace(i, st2.insert(i));

    EXPECT_EQ(st.lower_bound(6), nodes.find(7)->second);
    EXPECT_EQ(st.upper_bound(16), nodes.find(18)->second);
    EXPECT_EQ(st.lower_bound(5), nodes.find(5)->second);

    EXPECT_EQ(st1.lower_bound(8), nodes1.find(10)->second);
    EXPECT_EQ(st1.upper_bound(31), nodes1.find(20)->second);

    EXPECT_EQ(st2.lower_bound(21), nodes2.find(21)->second);
    EXPECT_EQ(st2.upper_bound(44), nodes2.find(45)->second);
    EXPECT_EQ(st2.lower_bound(-1), nodes2.find(0)->second);
    EXPECT_EQ(st2.upper_bound(100), nodes2.find(99)->second);
}

TEST(SearchTree, GetDistance)
{
    RangeQueries::search_tree_t<int> st{}, st1{};
    std::vector<int> input{15, 4, 5, 13, 2, 18, 14, 10, 7},
        input1{0, 44, 45, 1, 4, 5, 49, 57, 61, 72, 84, 99, 21, 25, 36, 46};
    for (auto i : input)
        st.insert(i);
    for (auto i : input1)
        st1.insert(i);

    EXPECT_EQ(st.get_distance(st.lower_bound(6), st.upper_bound(16)), 5);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(21), st1.upper_bound(44)), 4);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(-1), st1.upper_bound(100)),
              input1.size());
    EXPECT_EQ(st1.get_distance(st1.lower_bound(43), st1.upper_bound(72)), 7);
}
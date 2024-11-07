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
    EXPECT_EQ(n1->left(), st.end());
    EXPECT_EQ(n1->right(), st.end());
    EXPECT_EQ(n2->parent(), st.end());
    EXPECT_EQ(n2->left(), n1);
    EXPECT_EQ(n2->right(), n3);
    EXPECT_EQ(n3->parent(), n2);
    EXPECT_EQ(n3->right(), st.end());
    EXPECT_EQ(n3->left(), st.end());

    EXPECT_EQ(n4->parent(), n6);
    EXPECT_EQ(n4->right(), st1.end());
    EXPECT_EQ(n4->left(), st1.end());
    EXPECT_EQ(n5->parent(), n6);
    EXPECT_EQ(n5->right(), st1.end());
    EXPECT_EQ(n5->left(), st1.end());
    EXPECT_EQ(n6->parent(), st1.end());
    EXPECT_EQ(n6->right(), n4);
    EXPECT_EQ(n6->left(), n5);
}

TEST(SearchTree, Bounds)
{
    RangeQueries::search_tree_t<int> st{};
    std::vector<int> input{15, 4, 5, 13, 2, 18, 14, 10, 7};
    std::unordered_map<int, It> nodes;
    for (auto i : input)
        nodes.emplace(i, st.insert(i));

    EXPECT_EQ(st.lower_bound(6), nodes.find(5)->second);
    EXPECT_EQ(st.upper_bound(16), nodes.find(18)->second);
}

TEST(SearchTree, GetDistance)
{
    RangeQueries::search_tree_t<int> st{};
    std::vector<int> input{15, 4, 5, 13, 2, 18, 14, 10, 7};
    for (auto i : input)
        st.insert(i);

    EXPECT_EQ(st.get_distance(st.lower_bound(6), st.upper_bound(16)), 5);
}
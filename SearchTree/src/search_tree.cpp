#include "search_tree.h"
#include "range_queries.h"
#include "search_tree_exceptions.h"

#include <vector>
#include <iterator>
#include <iostream>

using search_tree_t = typename RangeQueries::search_tree_t<int>;

int main(void)
{
    RangeQueries::search_tree_t<int> st{};
    char request;
    for (std::cin >> request; !std::cin.eof(); std::cin >> request)
    {
        if (!std::cin.good())
            throw STExcepts::wrong_input("Input error: request type.");

        auto res = handle_request(st, request);
        if (res.has_value())
            std::cout << *res << " ";
    }
    std::cout << std::endl;
}
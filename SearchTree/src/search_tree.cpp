#include "search_tree.h"
#include "range_queries.h"
#include "search_tree_exceptions.h"

#include <iostream>
#include <iterator>
#include <vector>

using search_tree_t = typename RangeQueries::search_tree_t<int>;

int main(void)
{
    RangeQueries::search_tree_t<int> st{};
    char request;
    for (std::cin >> request; !std::cin.eof(); std::cin >> request)
    {
        if (!std::cin.good())
        {
            std::cerr << "Input error: request type." << std::endl;
            continue;
        }

        try
        {
            auto res = handle_request(st, request);
            if (res.has_value())
                std::cout << *res << " ";
        } catch (const STExcepts::wrong_input &wi)
        {
            std::cerr << wi.what() << std::endl;
            continue;
        } catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return 0;
        }
    }
    std::cout << std::endl;
}
#pragma once

#include "search_tree_exceptions.h"

#include <iostream>
#include <iterator>
#include <optional>

namespace RangeQueries {

template <typename SetT>
std::optional<size_t> handle_request(SetT &st, char request,
                                     std::istream &is = std::cin)
{
    switch (request)
    {
    case 'k':
    {
        int key;
        is >> key;
        if (!is.good() && !is.eof())
            throw STExcepts::wrong_input("Input error: key.");

        st.insert(key);
        break;
    }
    case 'q':
    {
        int left, right;
        is >> left >> right;
        if (!is.good() && !is.eof())
            throw STExcepts::wrong_input("Input error: bounds.");

        if (left > right)
            return 0;

        auto left_it = st.lower_bound(left), right_it = st.upper_bound(right);
        return st.get_distance(left_it, right_it);
        break;
    }
    default:
        throw STExcepts::wrong_input("Input error: request type.");
        break;
    }
    return std::optional<size_t>{};
}

template <std::random_access_iterator RandomIt>
void rq_update_sizes(RandomIt start, RandomIt end)
{
    for (; start != end; ++start)
        (*start)->sz++;
}

} // namespace RangeQueries
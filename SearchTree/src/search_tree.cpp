#include "search_tree.h"

#include <vector>
#include <iterator>
#include <iostream>

using search_tree_t = typename RangeQueries::search_tree_t<int>;

enum rq_error_t {
    NO_ERR = 0,
    REQUEST_TYPE = 1 << 0,
    KEY = 1 << 1,
    BOUNDS = 1 << 2,
};

rq_error_t handle_request(search_tree_t &st, char request)
{
    switch (request)
    {
    case 'k':
    {
        int key;
        std::cin >> key;
        if (!std::cin.good() && !std::cin.eof())
            return rq_error_t::KEY;

        st.insert(key);

#ifndef NDEBUG
        if (!st.valid())
        {
            std::cout << "Inserting " << key << "." << std::endl;
            st.dump();
            abort();
        }
#endif

        break;
    }
    case 'q':
    {
        int left, right;
        std::cin >> left >> right;
        if (!std::cin.good() && !std::cin.eof())
            return rq_error_t::BOUNDS;

        if (left > right)
        {
            std::cout << "0 ";
            break;
        }

        using ConstIt = typename RangeQueries::search_tree_t<int>::NodeConstIt;
        ConstIt left_it = st.lower_bound(left),
                right_it = st.upper_bound(right);
        std::cout << st.get_distance(left_it, right_it) << " ";
        break;
    }
    default:
        std::cout << "Incorrect input.\nType 'k' to input key and 'q' to input "
                     "bounds."
                  << std::endl;
        break;
    }
    return NO_ERR;
}

rq_error_t print_error(rq_error_t err)
{
    switch (err)
    {
    case rq_error_t::REQUEST_TYPE:
        std::cerr << "Input error: request type." << std::endl;
        break;

    case rq_error_t::KEY:
        std::cerr << "Input error: key." << std::endl;
        break;

    case rq_error_t::BOUNDS:
        std::cerr << "Input error: bounds." << std::endl;
        break;

    default:
        assert(0 && "UNREACHABLE");
        break;
    }
    return err;
}

int main(void)
{
    RangeQueries::search_tree_t<int> st{};
    char request;
    for (std::cin >> request; !std::cin.eof(); std::cin >> request)
    {
        if (!std::cin.good())
            return print_error(rq_error_t::REQUEST_TYPE);

        rq_error_t err;
        if (err = handle_request(st, request))
            return print_error(err);
    }
    std::cout << std::endl;
}
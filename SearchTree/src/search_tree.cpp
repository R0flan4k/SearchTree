#include "search_tree.h"

#include <vector>
#include <iterator>
#include <iostream>

int main(void)
{
    RangeQueries::search_tree_t<int> st{};
    for (char request = *(std::istream_iterator<char>(std::cin)); 
         request != EOF; std::cin >> request)
    {
        if (!std::cin.good())
        {
            std::cerr << "Input error: request type." << std::endl;
            return 1;
        }

        switch (request)
        {
            case 'k':
            {
                int key;
                std::cin >> key;
                if (!std::cin.good())
                {
                    std::cerr << "Input error: key." << std::endl;
                    return 1;
                }
                st.insert(key);
                break;
            }
            case 'q':
            {
                int left, right;
                std::cin >> left >> right;
                if (!std::cin.good())
                {
                    std::cerr << "Input error: bounds." << std::endl;
                    return 1;
                }
                using ConstIt = typename RangeQueries::search_tree_t<int>::NodeConstIt;
                ConstIt left_it = st.lower_bound(left), right_it = st.upper_bound(right);
                std::cout << st.get_distance(left_it, right_it) << " ";
            }
        }
    }
    std::cout << std::endl;
}
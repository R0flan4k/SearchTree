#pragma once

#include <stdexcept>

namespace STExcepts {

class search_tree_error : public std::runtime_error {
public:
    search_tree_error(const std::string &what_arg)
        : std::runtime_error(what_arg)
    {}
    virtual ~search_tree_error() = default;
};

class wrong_input final : public search_tree_error {
public:
    wrong_input(const std::string &what_arg) : search_tree_error(what_arg) {}
};

} // namespace STExcepts

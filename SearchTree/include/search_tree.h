#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <unordered_map>

namespace RangeQueries{

template <
    typename KeyT, 
    class CompT = std::less<KeyT>, 
    class KeyEqualT = std::equal_to<KeyT>
> class search_tree_t final {
    class node_t final {
    public:
        enum node_colors { RED, BLACK };
        using NodeIt = typename std::list<node_t>::iterator;
        using NodeConstIt = typename std::list<node_t>::const_iterator;

    private:
        KeyT key_;
        node_colors color_;
        size_t sz_;
        NodeIt parent_, left_, right_;
    
    public:
        node_t(const KeyT &key, node_colors color, NodeIt parent, NodeIt left,
               NodeIt right)
            : key_(key), color_(color), sz_(1), parent_(parent), left_(left),
              right_(right)
        {}

        NodeIt parent() const {return parent_;}
        NodeIt left() const {return left_;}
        NodeIt right() const {return right_;}
        const KeyT& key() const {return key_;}
        node_colors color() const {return color_;}
        size_t size() const { return sz_; }

        void set_parent(NodeIt it) {parent_ = it;}
        void set_left(NodeIt it) {left_ = it;}
        void set_right(NodeIt it) {right_ = it;}
        void set_key(KeyT &key) {key_ = key;}
        void set_color(node_colors color) {color_ = color;}
        void set_size(size_t sz) { sz_ = sz; }
        void inc_size() { ++sz_; }
        void dec_size() { --sz_; }
    };

public:
    using node_colors = typename node_t::node_colors;
    using NodeIt = typename node_t::NodeIt;
    using NodeConstIt = typename node_t::NodeConstIt;

    NodeIt nil() { return nodes_.end(); }
    NodeConstIt nil() const { return nodes_.end(); }

private:
    std::list<node_t> nodes_;
    NodeIt root_;

    void rotate_node_left(NodeIt node)
    {
        NodeIt end = nil();
        assert(node != end);
        NodeIt last_r = node->right();
        assert(last_r != end);

        node->set_right(last_r->left());
        if (last_r->left() != end)
            last_r->left()->set_parent(node);
        last_r->set_parent(node->parent());

        if (node->parent() == end)
            root_ = last_r;
        else if (node == node->parent()->left())
            node->parent()->set_left(last_r);
        else
            node->parent()->set_right(last_r);

        last_r->set_left(node);
        node->set_parent(last_r);

        size_t node_sz = (node->left() != end ? node->left()->size() : 0) +
                         (node->right() != end ? node->right()->size() : 0) + 1,
               last_r_sz =
                   (last_r->right() != end ? last_r->right()->size() : 0) +
                   node_sz + 1;
        node->set_size(node_sz);
        last_r->set_size(last_r_sz);
    }

    void rotate_node_right(NodeIt node)
    {
        NodeIt end = nil();
        assert(node != end);
        NodeIt last_l = node->left();
        assert(last_l != end);

        node->set_left(last_l->right());
        if (last_l->right() != end)
            last_l->right()->set_parent(node);
        last_l->set_parent(node->parent());

        if (node->parent() == end)
            root_ = last_l;
        else if (node == node->parent()->right())
            node->parent()->set_right(last_l);
        else
            node->parent()->set_left(last_l);

        last_l->set_right(node);
        node->set_parent(last_l);

        size_t node_sz = (node->left() != end ? node->left()->size() : 0) +
                         (node->right() != end ? node->right()->size() : 0) + 1,
               last_l_sz =
                   (last_l->left() != end ? last_l->left()->size() : 0) +
                   node_sz + 1;
        node->set_size(node_sz);
        last_l->set_size(last_l_sz);
    }

    void insert_balance(NodeIt node)
    {
        NodeIt end = nil();
        if (node->parent() != end)
        {
            while (node->parent() != end &&
                   node->parent()->color() == node_colors::RED)
            {
                if (node->parent() == node->parent()->parent()->left()) 
                {
                    NodeIt ppr = node->parent()->parent()->right();
                    if (ppr != end && ppr->color() == node_colors::RED)
                    {
                        node->parent()->set_color(node_colors::BLACK);
                        ppr->set_color(node_colors::BLACK);
                        node->parent()->parent()->set_color(node_colors::RED);
                        node = node->parent()->parent(); 
                    }
                    else if (node == node->parent()->right())
                    {
                        node = node->parent();
                        rotate_node_left(node);
                    }
                    else
                    {
                        node->parent()->set_color(node_colors::BLACK);
                        node->parent()->parent()->set_color(node_colors::RED);
                        rotate_node_right(node->parent()->parent());
                    }
                }
                else
                {
                    NodeIt ppl = node->parent()->parent()->left();
                    if (ppl != end && ppl->color() == node_colors::RED)
                    {
                        node->parent()->set_color(node_colors::BLACK);
                        ppl->set_color(node_colors::BLACK);
                        node->parent()->parent()->set_color(node_colors::RED);
                        node = node->parent()->parent(); 
                    }
                    else if (node == node->parent()->left())
                    {
                        node = node->parent();
                        rotate_node_right(node);
                    }
                    else
                    {
                        node->parent()->set_color(node_colors::BLACK);
                        node->parent()->parent()->set_color(node_colors::RED);
                        rotate_node_left(node->parent()->parent());
                    }
                }
            }
        }
        root_->set_color(node_colors::BLACK);
    }

    bool is_in_interval(const KeyT &key, const KeyT &left, const KeyT &right) const
    {
        return CompT{}(left, key) && CompT{}(key, right);
    }

    bool is_valid(NodeConstIt node, const KeyT &left, const KeyT &right) const
    {
        NodeConstIt end = nil();
        return node != end && is_in_interval(node->key(), left, right);
    }

    NodeConstIt get_sub_root(const KeyT &l_key, const KeyT &r_key) const
    {
        NodeConstIt sub_root = root_;
        while (true)
        {
            if (CompT{}(sub_root->key(), l_key))
                sub_root = sub_root->right();
            else if (CompT{}(r_key, sub_root->key()))
                sub_root = sub_root->left();
            else
                break;
        }
        return sub_root;
    }

    size_t get_distance_to_left(NodeConstIt sub_root, NodeConstIt l) const
    {
        if (l == sub_root)
            return 1;

        NodeConstIt end = nil(), par = sub_root, cur_node;
        KeyT l_key = l->key();
        size_t l_dist = 0;

        cur_node = sub_root->left();
        while (par != l)
        {
            if (CompT{}(cur_node->key(), l_key))
            {
                par = cur_node;
                cur_node = cur_node->right();
            }
            else
            {
                if (cur_node->right() != end)
                    l_dist += cur_node->right()->size();
                ++l_dist;
                par = cur_node;
                cur_node = cur_node->left();
            }
        }
        return l_dist;
    }

    size_t get_distance_to_right(NodeConstIt sub_root, NodeConstIt r) const
    {
        if (r == sub_root)
            return 1;

        NodeConstIt end = nil(), par = sub_root, cur_node;
        KeyT r_key = r->key();
        size_t r_dist = 0;

        cur_node = sub_root->right();
        while (par != r)
        {
            if (CompT{}(r_key, cur_node->key()))
            {
                par = cur_node;
                cur_node = cur_node->left();
            }
            else
            {
                if (cur_node->left() != end)
                    r_dist += cur_node->left()->size();
                ++r_dist;
                par = cur_node;
                cur_node = cur_node->right();
            }
        }
        return r_dist;
    }

    void dump_recursive(NodeIt node, size_t depth) const
    {
        if (node->right() != nil())
            dump_recursive(node->right(), depth + 1);

        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << "[" << node->key();
        if (node->color() == node_colors::BLACK)
            std::cout << "B";
        else
            std::cout << "R";
        std::cout << "]" << std::endl;
        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << "sz" << node->size() << std::endl;

        if (node->left() != nil())
            dump_recursive(node->left(), depth + 1);
    }

public:
    search_tree_t() : nodes_(), root_(nil()) {}

    NodeIt insert(const KeyT &key)
    {
        NodeIt end = nil();
        NodeIt par = end, cur_node = root_;
        
        while (cur_node != end)
        {
            if (KeyEqualT{}(key, cur_node->key()))
                return end;

            cur_node->inc_size();
            par = cur_node;
            if (CompT{}(key, cur_node->key()))
                cur_node = cur_node->left();
            else
                cur_node = cur_node->right();
        }

        nodes_.emplace_back(key, node_colors::RED, par, end, end);
        NodeIt new_node = std::prev(end);
        if (par == end)
            root_ = new_node;
        else if (CompT{}(key, par->key()))
            par->set_left(new_node);
        else
            par->set_right(new_node);

        insert_balance(new_node);
        return new_node;
    }

    NodeConstIt lower_bound(const KeyT &key) const
    {
        NodeConstIt end = nil();
        NodeConstIt par = end, cur_node = root_;

        while (cur_node != end)
        {
            if (CompT{}(key, cur_node->key()))
            {
                par = cur_node;
                cur_node = cur_node->left();
            }
            else if (KeyEqualT{}(key, cur_node->key()))
                return cur_node;
            else
            {
                par = cur_node;
                cur_node = cur_node->right();
            }
        }

        return par;
    }

    NodeConstIt upper_bound(const KeyT &key) const
    {
        NodeConstIt end = nil();
        NodeConstIt par = end, cur_node = root_;

        while (cur_node != end)
        {
            if (CompT{}(key, cur_node->key()))
            {
                par = cur_node;
                cur_node = cur_node->left();
            }
            else
            {
                par = cur_node;
                cur_node = cur_node->right();
            }
        }

        return par;
    }

    size_t get_distance(NodeConstIt s, NodeConstIt f) const
    {
        NodeConstIt end = nil();
        if (s == end || f == end || s == f)
            return 0;
        KeyT s_key = s->key(), f_key = f->key();
        assert(CompT{}(s_key, f_key));

        NodeConstIt sub_root = get_sub_root(s_key, f_key);

        size_t s_dist = get_distance_to_left(sub_root, s),
               f_dist = get_distance_to_right(sub_root, f);

        return s_dist + f_dist;
    }

    void dump() const
    {
        size_t start_depth = 0;
        dump_recursive(root_, start_depth);
    }
};

}
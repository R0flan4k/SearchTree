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
        // Changing color
    };

public:
    using node_colors = typename node_t::node_colors;
    using NodeIt = typename node_t::NodeIt;
    using NodeConstIt = typename node_t::NodeConstIt;

private:
    std::list<node_t> nodes_;
    NodeIt root_;

    void rotate_node_left(NodeIt node)
    {
        NodeIt end = nodes_.end();
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
        NodeIt end = nodes_.end();
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
        NodeIt end = nodes_.end();
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
        NodeConstIt end = nodes_.end();
        return node != end && is_in_interval(node->key(), left, right);
    }

    void dump_recursive(NodeIt node, size_t depth) const
    {
        if (node->right() != end())
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

        if (node->left() != end())
            dump_recursive(node->left(), depth + 1);
    }

public:
    search_tree_t() : nodes_(), root_(nodes_.end()) {}

    NodeIt end() {return nodes_.end();}
    NodeConstIt end() const { return nodes_.end(); }

    NodeIt insert(const KeyT &key)
    {
        NodeIt end = nodes_.end();
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
        NodeIt new_node = std::prev(nodes_.end());
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
        NodeConstIt end = nodes_.end();
        NodeConstIt par = end, cur_node = root_;

        while (cur_node != end && CompT{}(key, cur_node->key()))
        {
            par = cur_node;
            cur_node = cur_node->left();
        }
        if (cur_node == end) return par;

        while (cur_node != end && !CompT{}(key, cur_node->key()))
        {
            par = cur_node;
            cur_node = cur_node->right();
        }

        return par;
    }

    NodeConstIt upper_bound(const KeyT &key) const
    {
        NodeConstIt end = nodes_.end();
        NodeConstIt par = end, cur_node = root_;

        while (cur_node != end && !CompT{}(key, cur_node->key()))
        {
            par = cur_node;
            cur_node = cur_node->right();
        }
        if (cur_node == end) return par;

        while (cur_node != end && CompT{}(key, cur_node->key()))
        {
            par = cur_node;
            cur_node = cur_node->left();
        }

        return par;
    }

    size_t get_distance(NodeConstIt s, NodeConstIt f) const
    {
        NodeConstIt end = nodes_.end();
        if (s == end || f == end)
            return 0;

        assert(CompT{}(s->key(), f->key()));

        KeyT s_key = s->key(), f_key = f->key();
        size_t s_l_size = (s->left() != end ? s->left()->size() : 0),
               f_r_size = (f->right() != end ? f->right()->size() : 0);
        NodeConstIt subtree_root = s, par = s->parent();

        while (par != end && is_valid(par, s_key, f_key))
        {
            subtree_root = par;
            par = par->parent();
        }

        return subtree_root->size() - s_l_size - f_r_size - 2;
    }

    void dump() const
    {
        size_t start_depth = 0;
        dump_recursive(root_, start_depth);
    }
};

}
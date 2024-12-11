#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>

namespace RangeQueries{

template <
    typename KeyT, 
    class CompT = std::less<KeyT>, 
    class KeyEqualT = std::equal_to<KeyT>
> class search_tree_t final {
    struct node_t final {
        enum class node_colors : bool { RED, BLACK };
        using NodeIt = typename std::list<node_t>::iterator;
        using NodeConstIt = typename std::list<node_t>::const_iterator;

        KeyT key;
        node_colors color;
        NodeIt parent, left, right;
        size_t sz = 1;
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

    size_t subtree_size(NodeIt node) const
    {
        return node != nil() ? node->sz : 0;
    }

    void rotate_node_left(NodeIt node)
    {
        NodeIt end = nil();
        assert(node != end);
        NodeIt last_r = node->right;
        assert(last_r != end);

        node->right = last_r->left;
        if (last_r->left != end)
            last_r->left->parent = node;
        last_r->parent = node->parent;

        if (node->parent == end)
            root_ = last_r;
        else if (node == node->parent->left)
            node->parent->left = last_r;
        else
            node->parent->right = last_r;

        last_r->left = node;
        node->parent = last_r;

        node->sz = subtree_size(node->left) + subtree_size(node->right) + 1;
        last_r->sz = subtree_size(last_r->right) + node->sz + 1;
    }

    void rotate_node_right(NodeIt node)
    {
        NodeIt end = nil();
        assert(node != end);
        NodeIt last_l = node->left;
        assert(last_l != end);

        node->left = last_l->right;
        if (last_l->right != end)
            last_l->right->parent = node;
        last_l->parent = node->parent;

        if (node->parent == end)
            root_ = last_l;
        else if (node == node->parent->right)
            node->parent->right = last_l;
        else
            node->parent->left = last_l;

        last_l->right = node;
        node->parent = last_l;

        node->sz = subtree_size(node->left) + subtree_size(node->right) + 1;
        last_l->sz = subtree_size(last_l->left) + node->sz + 1;
    }

    void insert_balance_repaint_uncle_is_red(NodeIt node, NodeIt uncle)
    {
        node->parent->color = node_colors::BLACK;
        uncle->color = node_colors::BLACK;
        node->parent->parent->color = node_colors::RED;
    }

    // void insert_balance_repaint_p_is_left(NodeIt &node, NodeIt uncle)
    // {
    //     if (uncle != nil() && uncle->color == node_colors::RED)
    //     {
    //         insert_balance_repaint_uncle_is_red(node, uncle);
    //         node = node->parent->parent;
    //     }
    //     else if (node == node->parent->right)
    //     {
    //         node = node->parent;
    //         rotate_node_left(node);
    //     }
    //     else
    //     {
    //         node->parent->color = node_colors::BLACK;
    //         node->parent->parent->color = node_colors::RED;
    //         rotate_node_right(node->parent->parent);
    //     }
    // }

    // void insert_balance_repaint_p_is_right(NodeIt &node, NodeIt uncle)
    // {
    //     if (uncle != nil() && uncle->color == node_colors::RED)
    //     {
    //         insert_balance_repaint_uncle_is_red(node, uncle);
    //         node = node->parent->parent;
    //     }
    //     else if (node == node->parent->left)
    //     {
    //         node = node->parent;
    //         rotate_node_right(node);
    //     }
    //     else
    //     {
    //         node->parent->color = node_colors::BLACK;
    //         node->parent->parent->color = node_colors::RED;
    //         rotate_node_left(node->parent->parent);
    //     }
    // }

    template <class rotation_policy>
    void insert_balance_repaint(NodeIt &node, NodeIt uncle)
    {
        if (uncle != nil() && uncle->color == node_colors::RED)
        {
            insert_balance_repaint_uncle_is_red(node, uncle);
            node = node->parent->parent;
        }
        else if (node == rotation_policy::different_side_child(node))
        {
            node = node->parent;
            // rotate_node_right(node);
            rotation_policy::different_side_child_rotate(*this, node);
        }
        else
        {
            node->parent->color = node_colors::BLACK;
            node->parent->parent->color = node_colors::RED;
            // rotate_node_left(node->parent->parent);
            rotation_policy::residual_rotate(*this, node);
        }
    }

    struct repaint_rotate_parent_is_left {
        static NodeIt different_side_child(NodeIt node)
        {
            return node->parent->right;
        }

        static void different_side_child_rotate(search_tree_t &t, NodeIt node)
        {
            t.rotate_node_left(node);
        }

        static void residual_rotate(search_tree_t &t, NodeIt node)
        {
            t.rotate_node_right(node->parent->parent);
        }
    };

    struct repaint_rotate_parent_is_right {
        static NodeIt different_side_child(NodeIt node)
        {
            return node->parent->left;
        }

        static void different_side_child_rotate(search_tree_t &t, NodeIt node)
        {
            t.rotate_node_right(node);
        }

        static void residual_rotate(search_tree_t &t, NodeIt node)
        {
            t.rotate_node_left(node->parent->parent);
        }
    };

    void insert_balance_repaint_p_is_left(NodeIt &node, NodeIt uncle)
    {
        insert_balance_repaint<repaint_rotate_parent_is_left>(node, uncle);
    }

    void insert_balance_repaint_p_is_right(NodeIt &node, NodeIt uncle)
    {
        insert_balance_repaint<repaint_rotate_parent_is_right>(node, uncle);
    }

    void insert_balance(NodeIt node)
    {
        NodeIt end = nil();
        if (node->parent != end)
        {
            while (node->parent != end &&
                   node->parent->color == node_colors::RED)
            {
                if (node->parent == node->parent->parent->left)
                {
                    NodeIt uncle = node->parent->parent->right;
                    insert_balance_repaint_p_is_left(node, uncle);
                }
                else
                {
                    NodeIt uncle = node->parent->parent->left;
                    insert_balance_repaint_p_is_right(node, uncle);
                }
            }
        }
        root_->color = node_colors::BLACK;
    }

    NodeConstIt get_sub_root(NodeConstIt node) const
    {
        NodeConstIt end = nil();
        if (node == end)
            return root_;

        const KeyT &n_key = node->key;
        NodeConstIt sub_root = root_;
        while (CompT{}(sub_root->key, n_key))
            sub_root = sub_root->right;

        return sub_root;
    }

    NodeConstIt get_sub_root(NodeConstIt l, NodeConstIt r) const
    {
        NodeConstIt end = nil();
        if (r == end)
            return get_sub_root(l);

        const KeyT &l_key = l->key, r_key = r->key;
        NodeConstIt sub_root = root_;
        while (true)
        {
            if (CompT{}(sub_root->key, l_key))
                sub_root = sub_root->right;
            else if (CompT{}(r_key, sub_root->key))
                sub_root = sub_root->left;
            else
                break;
        }
        return sub_root;
    }

    size_t get_distance_to_left(NodeConstIt sub_root, NodeConstIt l) const
    {
        if (l == sub_root)
            return 0;

        NodeConstIt end = nil(), par = sub_root, cur_node;
        KeyT l_key = l->key;
        size_t l_dist = 0;

        cur_node = sub_root->left;
        while (par != l)
        {
            if (CompT{}(cur_node->key, l_key))
            {
                par = cur_node;
                cur_node = cur_node->right;
            }
            else
            {
                if (cur_node->right != end)
                    l_dist += cur_node->right->sz;
                ++l_dist;
                par = cur_node;
                cur_node = cur_node->left;
            }
        }
        return l_dist;
    }

    size_t get_distance_to_right(NodeConstIt sub_root, NodeConstIt r) const
    {
        if (r == sub_root)
            return 0;
        NodeConstIt end = nil(), par = sub_root, cur_node;
        if (r == end)
            return subtree_size(sub_root->right) + 1;

        KeyT r_key = r->key;
        size_t r_dist = 0;

        cur_node = sub_root->right;
        while (par != r)
        {
            if (CompT{}(r_key, cur_node->key))
            {
                par = cur_node;
                cur_node = cur_node->left;
            }
            else
            {
                if (cur_node->left != end)
                    r_dist += cur_node->left->sz;
                ++r_dist;
                par = cur_node;
                cur_node = cur_node->right;
            }
        }
        return r_dist;
    }

    void dump_recursive(NodeIt node, size_t depth) const
    {
        if (node->right != nil())
            dump_recursive(node->right, depth + 1);

        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << "[" << node->key;
        if (node->color == node_colors::BLACK)
            std::cout << "B";
        else
            std::cout << "R";
        std::cout << "]" << std::endl;
        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << "sz" << node->sz << std::endl;

        if (node->left != nil())
            dump_recursive(node->left, depth + 1);
    }

    template <std::random_access_iterator RandomIt>
    void insert_update_sizes(RandomIt start, RandomIt end)
    {
        for (; start != end; ++start)
            (*start)->sz++;
    }

    struct NodeCompT {
        constexpr bool operator()(NodeConstIt lhs, NodeConstIt rhs) const
        {
            return CompT{}(lhs->key, rhs->key);
        }
    };

    bool valid_rec(NodeConstIt node) const
    {
        NodeConstIt end = nil();
        if (node->left != end && !valid_rec(node->left))
            return false;
        if (node->right != end && !valid_rec(node->right))
            return false;
        size_t r_sz = subtree_size(node->right),
               l_sz = subtree_size(node->left);
        return node->sz == r_sz + l_sz + 1;
    }

public:
    search_tree_t() : nodes_(), root_(nil()) {}

    size_t size() const { return root_->sz; }

    NodeConstIt find(const KeyT &key)
    {
        NodeConstIt end = nil();
        NodeConstIt par = end, cur_node = root_;

        while (cur_node != end)
        {
            if (KeyEqualT{}(key, cur_node->key))
                return cur_node;

            if (CompT{}(key, cur_node->key))
                cur_node = cur_node->left;
            else
                cur_node = cur_node->right;
        }
        return end;
    }

    NodeIt insert(const KeyT &key)
    {
        NodeIt end = nil();
        NodeIt par = end, cur_node = root_;
        // We use this vector to increment sizes of nodes
        // that contain new node. If node with this key
        // already exists we don't increment sizes.
        std::vector<NodeIt> nodes_to_inc;

        while (cur_node != end)
        {
            if (KeyEqualT{}(key, cur_node->key))
                return cur_node;

            nodes_to_inc.push_back(cur_node);
            par = cur_node;
            if (CompT{}(key, cur_node->key))
                cur_node = cur_node->left;
            else
                cur_node = cur_node->right;
        }

        nodes_.emplace_back(key, node_colors::RED, par, end, end);
        NodeIt new_node = std::prev(end);
        if (par == end)
            root_ = new_node;
        else if (CompT{}(key, par->key))
            par->left = new_node;
        else
            par->right = new_node;

        insert_update_sizes(nodes_to_inc.begin(), nodes_to_inc.end());
        insert_balance(new_node);
        return new_node;
    }

    NodeConstIt lower_bound(const KeyT &key) const
    {
        NodeConstIt end = nil();
        NodeConstIt res = end, cur_node = root_;

        while (cur_node != end)
        {
            if (CompT{}(key, cur_node->key))
            {
                res = (res == end) ? cur_node
                                   : std::min(cur_node, res, NodeCompT{});
                cur_node = cur_node->left;
            }
            else if (KeyEqualT{}(key, cur_node->key))
                return cur_node;
            else
                cur_node = cur_node->right;
        }

        return res;
    }

    NodeConstIt upper_bound(const KeyT &key) const
    {
        NodeConstIt end = nil();
        NodeConstIt res = end, cur_node = root_;

        while (cur_node != end)
        {
            if (CompT{}(key, cur_node->key))
            {
                res = (res == end) ? cur_node
                                   : std::min(cur_node, res, NodeCompT{});
                cur_node = cur_node->left;
            }
            else
                cur_node = cur_node->right;
        }

        return res;
    }

    size_t get_distance(NodeConstIt s, NodeConstIt f) const
    {
        if (s == f)
            return 0;
        NodeConstIt end = nil();
        assert(f == end ? s != end : CompT{}(s->key, f->key));

        NodeConstIt sub_root = get_sub_root(s, f);
        size_t s_dist = get_distance_to_left(sub_root, s),
               f_dist = get_distance_to_right(sub_root, f);

        return s_dist + f_dist;
    }

    void dump() const
    {
        size_t start_depth = 0;
        dump_recursive(root_, start_depth);
    }

    bool valid() const { return valid_rec(root_); }
};

}
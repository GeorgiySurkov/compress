#pragma once

#include <iostream>
#include "utility.h"

namespace mstd {

    template<typename T>
    struct less {
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
    };

    //--------------------------------
    // map
    //--------------------------------
    template<typename Key, typename T, typename Compare = less<Key>>
    class map {
    public:
        // TODO: key should be const
//        using value_type = mstd::pair<const Key, T>;
        using value_type = mstd::pair<Key, T>;

    private:
        struct Node {
            value_type value;
            Node *parent;
            char color;
            Node *left;
            Node *right;

            Node *minimum() {
                Node *now = this;
                while (now->left) now = now->left;
                return now;
            }

            Node *maximum() {
                Node *now = this;
                while (now->right) now = now->right;
                return now;
            }

            Node *next() {
                if (right) return right->minimum();
                Node *p = parent, *now = this;
                while (p) {
                    if (now == p->left) break;
                    now = p;
                    p = p->parent;
                }
                return p;
            }

            Node *prev() {
                if (left) return left->maximum();
                Node *p = parent, *now = this;
                while (p) {
                    if (now == p->right) break;
                    now = p;
                    p = p->parent;
                }
                return p;
            }
        };

        Compare _comp;
        size_t _size;
        Node *_root;

    public:
        //--------------------------------
        // Constructors and destructor
        //--------------------------------
        explicit map(const Compare &comp = Compare()) : _comp(comp), _size(0), _root(nullptr) {}

        ~map() { clear(); }

        //--------------------------------
        // Copy and move constructors
        //--------------------------------
        map(const map &m) {
            _comp = m._comp;
            _size = m._size;
            _root = clone_node(m._root);
        }

        map(map &&m) noexcept {
            _comp = std::move(m._comp);
            _size = m._size;
            _root = m._root;
            m._root = nullptr;
        }

        //--------------------------------
        // Copy and move assignment
        //--------------------------------
        map &operator=(const map &m) {
            if (this == &m) { return *this; }
            _comp = m._comp;
            _size = m._size;
            delete_node(_root);
            _root = clone_node(m._root);
        }

        map &operator=(map &&m) noexcept {
            if (this == &m) { return *this; }
            _comp = std::move(m._comp);
            _size = m._size;
            delete_node(_root);
            _root = m._root;
            m._root = nullptr;
        }

        //--------------------------------
        // Element access
        //--------------------------------
        T &operator[](const Key &key) {
            return insert({key, T()}).first->second;
//            auto it = find(key);
//            if (it == end()) {
//                insert(T());
//            }
//            return it->second;
        }

        const T &operator[](const Key &key) const {
            return find(key)->second;
        }

        //--------------------------------
        // Iterators
        //--------------------------------
    private:
        template<typename P>
        class iterator_template {
            Node *_curr;

            iterator_template(Node *curr) : _curr(curr) {}

        public:
            P &operator*() const { return _curr->value; }

            P *operator->() const { return &(_curr->value); }

            friend inline bool operator==(const iterator_template &lhs, const iterator_template &rhs) {
                return lhs._curr == rhs._curr;
            }

            friend inline bool operator!=(const iterator_template &lhs, const iterator_template &rhs) {
                return !operator==(lhs, rhs);
            }

            iterator_template &operator++() {
                if (_curr) _curr = _curr->next();
                return *this;
            }

            iterator_template operator++(int) {
                iterator_template old = *this;
                operator++();
                return old;
            }

            iterator_template &operator--() {
                if (_curr) _curr = _curr->prev();
                return *this;
            }

            iterator_template operator--(int) {
                iterator_template old = *this;
                operator--();
                return old;
            }

            friend class map;
        };

        template<typename P>
        class reverse_iterator_template {
            Node *_curr;

            reverse_iterator_template(Node *curr) : _curr(curr) {}

        public:
            P &operator*() const { return _curr->value; }

            P *operator->() const { return &(_curr->value); }

            friend inline bool operator==(const reverse_iterator_template &lhs, const reverse_iterator_template &rhs) {
                return lhs._curr == rhs._curr;
            }

            friend inline bool operator!=(const reverse_iterator_template &lhs, const reverse_iterator_template &rhs) {
                return !operator==(lhs, rhs);
            }

            reverse_iterator_template &operator++() {
                if (_curr) _curr = _curr->prev();
                return *this;
            }

            reverse_iterator_template operator++(int) {
                reverse_iterator_template old = *this;
                operator++();
                return old;
            }

            reverse_iterator_template &operator--() {
                if (_curr) _curr = _curr->next();
                return *this;
            }

            reverse_iterator_template operator--(int) {
                reverse_iterator_template old = *this;
                operator--();
                return old;
            }

            friend class map;
        };

    public:
        using iterator = iterator_template<value_type>;

        iterator begin() {
            if (_root) {
                return {_root->minimum()};
            }
            return end();
        }

        iterator end() {
            return {nullptr};
        }

        using const_iterator = iterator_template<const value_type>;

        const_iterator cbegin() const {
            if (_root) {
                return {_root->minimum()};
            }
            return cend();
        }

        const_iterator cend() const {
            return {nullptr};
        }

        using reverse_iterator = reverse_iterator_template<value_type>;

        reverse_iterator rbegin() {
            if (_root) {
                return {_root->maximum()};
            }
            return rend();
        }

        reverse_iterator rend() {
            return {nullptr};
        }

        using const_reverse_iterator = reverse_iterator_template<const value_type>;

        const_reverse_iterator crbegin() const {
            if (_root) {
                return {_root->maximum()};
            }
            return crend();
        }

        const_reverse_iterator crend() const {
            return {nullptr};
        }

        //--------------------------------
        // Capacity
        //--------------------------------
        [[nodiscard]] size_t size() const { return _size; }

        [[nodiscard]] bool empty() const { return _root == nullptr; }

        //--------------------------------
        // Modifiers
        //--------------------------------
        void clear();

        mstd::pair<iterator, bool> insert(const value_type &value);

        size_t erase(const Key &key);

        //--------------------------------
        // Lookup
        //--------------------------------
        iterator find(const Key &key);

        const_iterator find(const Key &key) const;

    private:
        void fix_insertion(Node *t);

        void fix_deletion(Node *p);

        void left_rotate(Node *p);

        void right_rotate(Node *p);

        void delete_node(Node *p);

        static Node *clone_node(const Node *p);

        static Node *successor(Node *p);
    };

    template<typename Key, typename T, typename Compare>
    pair<typename map<Key, T, Compare>::iterator, bool> map<Key, T, Compare>::insert(const map::value_type &value) {
        Node *p = _root, *q = nullptr;
        if (!_root) {
            _root = new Node{value, nullptr, 'r', nullptr, nullptr};
//            _root->value = value;
//            _root->left = nullptr;
//            _root->right = nullptr;
//            _root->color = 'r';
//            _root->parent = nullptr;
            fix_insertion(_root);
            ++_size;
            return {iterator(_root), true};
        } else {
            while (p != nullptr) {
                q = p;
//                if (p->key < t->key)
                if (_comp(p->value.first, value.first)) {
                    p = p->right;
                } else if (_comp(value.first, p->value.first)) {
                    p = p->left;
                } else {
                    return {iterator(p), false};
                }
            }
            Node *t = new Node{value, q, 'r', nullptr, nullptr};
//            t->value = value;
//            t->left = nullptr;
//            t->right = nullptr;
//            t->color = 'r';
//            t->parent = q;
//            if (q->key < t->key)
            if (_comp(q->value.first, t->value.first)) {
                q->right = t;
            } else {
                q->left = t;
            }
            ++_size;
            fix_insertion(t);
            return {iterator(t), true};
        }
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::fix_insertion(map::Node *t) {
        Node *u;
        if (_root == t) {
            t->color = 'b';
            return;
        }
        while (t->parent != nullptr && t->parent->color == 'r') {
            Node *g = t->parent->parent;
            if (g->left == t->parent) {
                if (g->right != nullptr) {
                    u = g->right;
                    if (u->color == 'r') {
                        t->parent->color = 'b';
                        u->color = 'b';
                        g->color = 'r';
                        t = g;
                    }
                } else {
                    if (t->parent->right == t) {
                        t = t->parent;
                        left_rotate(t);
                    }
                    t->parent->color = 'b';
                    g->color = 'r';
                    right_rotate(g);
                }
            } else {
                if (g->left != nullptr) {
                    u = g->left;
                    if (u->color == 'r') {
                        t->parent->color = 'b';
                        u->color = 'b';
                        g->color = 'r';
                        t = g;
                    }
                } else {
                    if (t->parent->left == t) {
                        t = t->parent;
                        right_rotate(t);
                    }
                    t->parent->color = 'b';
                    g->color = 'r';
                    left_rotate(g);
                }
            }
            _root->color = 'b';
        }
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::left_rotate(map::Node *p) {
//        if (p->right == nullptr)
//            return;
//        else {
//            Node *y = p->right;
//            if (y->left != nullptr) {
//                p->right = y->left;
//                y->left->parent = p;
//            } else {
//                p->right = nullptr;
//            }
//            if (p->parent != nullptr)
//                y->parent = p->parent;
//            if (p->parent == nullptr)
//                _root = y;
//            else {
//                if (p == p->parent->left)
//                    p->parent->left = y;
//                else
//                    p->parent->right = y;
//            }
//            y->left = p;
//            p->parent = y;
//        }
        Node *right_child = p->right;
        p->right = right_child->left;

        if (p->right != nullptr)
            p->right->parent = p;

        right_child->parent = p->parent;

        if (p->parent == nullptr)
            _root = right_child;
        else if (p == p->parent->left)
            p->parent->left = right_child;
        else
            p->parent->right = right_child;

        right_child->left = p;
        p->parent = right_child;
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::right_rotate(map::Node *p) {
//        if (p->left == nullptr)
//            return;
//        else {
//            Node *y = p->left;
//            if (y->right != nullptr) {
//                p->left = y->right;
//                y->right->parent = p;
//            } else
//                p->left = nullptr;
//            if (p->parent != nullptr)
//                y->parent = p->parent;
//            if (p->parent == nullptr)
//                _root = y;
//            else {
//                if (p == p->parent->left)
//                    p->parent->left = y;
//                else
//                    p->parent->right = y;
//            }
//            y->right = p;
//            p->parent = y;
//        }
        Node *left_child = p->left;
        p->left = left_child->right;

        if (p->left != nullptr)
            p->left->parent = p;

        left_child->parent = p->parent;

        if (p->parent == nullptr)
            _root = left_child;
        else if (p == p->parent->left)
            p->parent->left = left_child;
        else
            p->parent->right = left_child;

        left_child->right = p;
        p->parent = left_child;
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::delete_node(map::Node *p) {
        if (!p) {
            return;
        }
        delete_node(p->left);
        p->left = nullptr;
        delete_node(p->right);
        p->right = nullptr;
        delete p;
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::clear() {
        delete_node(_root);
        _root = nullptr;
        _size = 0;
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::Node *map<Key, T, Compare>::clone_node(const map::Node *p) {
        if (!p) {
            return nullptr;
        }
        Node *left = clone_node(p->left), *right = clone_node(p->right);
        Node *clone = new Node(*p);
        clone->left = left;
        clone->left->parent = clone;
        clone->right = right;
        clone->right->parent = clone;
        clone->parent = nullptr;
        return clone;
    }

    template<typename Key, typename T, typename Compare>
    size_t map<Key, T, Compare>::erase(const Key &key) {
        if (_root == nullptr) {
            return 0;
        }
        Node *p = _root, *y = nullptr, *q = nullptr;
        size_t found = 0;
        while (p != nullptr && found == 0) {
            // TODO: refactor
//            if (p->key == x)
            if (!_comp(p->value.first, key) && !_comp(key, p->value.first))
                found = 1;
            if (found == 0) {
//                if (p->key < x)
                if (_comp(p->value.first, key))
                    p = p->right;
                else if (_comp(key, p->value.first))
                    p = p->left;
            }
        }
        if (found == 0) {
            return found;
        } else {
            if (p->left == nullptr || p->right == nullptr)
                y = p;
            else
                y = successor(p);
            if (y->left != nullptr)
                q = y->left;
            else {
                if (y->right != nullptr)
                    q = y->right;
                else
                    q = nullptr;
            }
            if (q != nullptr)
                q->parent = y->parent;
            if (y->parent == nullptr)
                _root = q;
            else {
                if (y == y->parent->left)
                    y->parent->left = q;
                else
                    y->parent->right = q;
            }
            if (y != p) {
                p->color = y->color;
                p->value = y->value;
            }
            if (y->color == 'b')
                fix_deletion(q);
        }
        --_size;
        return found;
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::Node *map<Key, T, Compare>::successor(map::Node *p) {
        Node *y;
        if (p->left != nullptr) {
            y = p->left;
            while (y->right != nullptr)
                y = y->right;
        } else {
            y = p->right;
            while (y->left != nullptr)
                y = y->left;
        }
        return y;
    }

    template<typename Key, typename T, typename Compare>
    void map<Key, T, Compare>::fix_deletion(map::Node *p) {
        Node *s;
        while (p != _root && p->color == 'b') {
            if (p->parent->left == p) {
                s = p->parent->right;
                if (s->color == 'r') {
                    s->color = 'b';
                    p->parent->color = 'r';
                    left_rotate(p->parent);
                    s = p->parent->right;
                }
                if (s->right->color == 'b' && s->left->color == 'b') {
                    s->color = 'r';
                    p = p->parent;
                } else {
                    if (s->right->color == 'b') {
                        s->left->color = 'b';
                        s->color = 'r';
                        right_rotate(s);
                        s = p->parent->right;
                    }
                    s->color = p->parent->color;
                    p->parent->color = 'b';
                    s->right->color = 'b';
                    left_rotate(p->parent);
                    p = _root;
                }
            } else {
                s = p->parent->left;
                if (s->color == 'r') {
                    s->color = 'b';
                    p->parent->color = 'r';
                    right_rotate(p->parent);
                    s = p->parent->left;
                }
                if (s->left->color == 'b' && s->right->color == 'b') {
                    s->color = 'r';
                    p = p->parent;
                } else {
                    if (s->left->color == 'b') {
                        s->right->color = 'b';
                        s->color = 'r';
                        left_rotate(s);
                        s = p->parent->left;
                    }
                    s->color = p->parent->color;
                    p->parent->color = 'b';
                    s->left->color = 'b';
                    right_rotate(p->parent);
                    p = _root;
                }
            }
            p->color = 'b';
            _root->color = 'b';
        }
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::iterator map<Key, T, Compare>::find(const Key &key) {
        Node *now = _root;
        while (now) {
            if (_comp(now->value.first, key)) {
                now = now->left;
            } else if (_comp(key, now->value.first)) {
                now = now->right;
            } else {
                break;
            }
        }
        return iterator(now);
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::const_iterator map<Key, T, Compare>::find(const Key &key) const {
        Node *now = _root;
        while (now) {
            if (_comp(now->value.first, key)) {
                now = now->left;
            } else if (_comp(key, now->value.first)) {
                now = now->right;
            } else {
                break;
            }
        }
        return const_iterator(now);
    }

} // mstd

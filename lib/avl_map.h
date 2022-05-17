#pragma once

#include <iostream>
#include "./utility.h"
#include "./functional.h"

namespace mstd {

    //--------------------------------
    // map
    //--------------------------------
    template<typename Key, typename T, typename Compare = less<Key>>
    class map {
    public:
        using value_type = mstd::pair<const Key, T>;

    private:
        struct Node {
            Node *left, *right, *parent;
            value_type value;
            signed char balance;

            Node(const value_type &val) :
                    value(val),
                    left(nullptr),
                    right(nullptr),
                    parent(nullptr),
                    balance(0) {}

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

            [[nodiscard]] int level() const {
                int L = (left) ? left->level() : 0;
                int R = (right) ? right->level() : 0;
                return L > R ? L + 1 : R + 1;
            }
        };

        Compare _comp;
        size_t _size;
        Node *_root;

    public:
        //--------------------------------
        // Constructors and destructor
        //--------------------------------
        explicit map(const Compare &comp = Compare()) : _comp(comp), _root(nullptr), _size(0) {}

        ~map() {
            clear();
        }

        //--------------------------------
        // Copy and move constructors
        //--------------------------------
        map(const map &m) : _comp(m._comp), _size(m._size), _root(nullptr) {
            _root = clone_node(m._root);
        }

        map(map &&m) noexcept : _comp(std::move(m._comp)), _size(m._size), _root(m._root) {
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
        Node *left_rotate(Node *p);

        Node *right_rotate(Node *p);

        Node *double_left_rotate(Node *p);

        Node *double_right_rotate(Node *p);

        void delete_node(Node *p);

        static Node *clone_node(const Node *p);

    };

    template<typename Key, typename T, typename Compare>
    pair<typename map<Key, T, Compare>::iterator, bool> map<Key, T, Compare>::insert(const map::value_type &value) {
//        ++count;
        if (_root == nullptr) {
            _root = new Node(value);
            ++_size;
            return {iterator(_root), true};
        }
        Node *now, *prev;
        bool toLeft;
        now = _root;
        do {
            prev = now;
//            if (X < now->data) {
            if (_comp(value.first, now->value.first)) {
                now = now->left;
                toLeft = true;
            } else if (_comp(now->value.first, value.first)) {
                now = now->right;
                toLeft = false;
            } else {
                return {iterator(now), false};
            }
        } while (now);
        now = new Node(value);
        iterator res_iterator = iterator(now);
        ++_size;
        if (toLeft) prev->left = now; else prev->right = now;
        now->parent = prev;

        // fix balance
        do {
            if (now->parent->left == now) now->parent->balance -= 1;
            else now->parent->balance += 1;
            now = now->parent;
            switch (now->balance) {
                case 2:
                    if (now->right->balance == -1) now = double_left_rotate(now);
                    else now = left_rotate(now);
                    break;
                case -2:
                    if (now->left->balance == 1) now = double_right_rotate(now);
                    else now = right_rotate(now);
                    break;
            }
        } while (now != _root && now->balance != 0);
        return {res_iterator, true};
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::Node *map<Key, T, Compare>::left_rotate(map::Node *p) {
        if (p->balance != 2) return p;
        Node *b = p->right;
        if (b->balance == -1) return p;

        p->right = b->left;
        if (b->left) b->left->parent = p;

        b->parent = p->parent;
        if (p->parent) {
            if (p->parent->left == p)
                p->parent->left = b;
            else
                p->parent->right = b;
        } else {
            _root = b;
        }

        b->left = p;
        p->parent = b;

        if (b->balance == 1) {
            p->balance = 0;
            b->balance = 0;
        } else {
            p->balance = 1;
            b->balance = -1;
        }
        return b;
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::Node *map<Key, T, Compare>::right_rotate(map::Node *p) {
        if (p->balance != -2) return p;
        Node *a = p->left;
        if (a->balance == 1) return p;

        p->left = a->right;
        if (a->right) a->right->parent = p;

        a->parent = p->parent;
        if (p->parent) {
            if (p->parent->left == p)
                p->parent->left = a;
            else
                p->parent->right = a;
        } else {
            _root = a;
        }

        a->right = p;
        p->parent = a;

        if (a->balance == -1) {
            a->balance = 0;
            p->balance = 0;
        } else {
            a->balance = 1;
            p->balance = -1;
        }

        return a;
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
        iterator pos = find(key);
        Node *toDelete = pos._curr;
        if (toDelete == nullptr) return 0;
        Node *alt;
        if (toDelete->right == nullptr)
            alt = toDelete->left;
        else if (toDelete->left == nullptr)
            alt = toDelete->right;
        else {
            alt = toDelete->right->minimum();
            if (alt->parent != toDelete) {
                alt->parent->left = alt->right;
                if (alt->right) alt->right->parent = alt->parent;
                alt->right = toDelete->right;
                toDelete->right->parent = alt;
            }
            alt->left = toDelete->left;
            toDelete->left->parent = alt;
        }

        Node *notBalanced;
        if (alt == nullptr) {
            notBalanced = toDelete->parent;
            if (notBalanced) {
                if (notBalanced->left == toDelete) notBalanced->balance += 1;
                else notBalanced->balance -= 1;
            }
        } else {
            alt->balance = toDelete->balance;
            if (alt->parent == toDelete) {
                notBalanced = alt;
                if (alt == toDelete->left) notBalanced->balance += 1;
                else notBalanced->balance -= 1;
            } else {
                notBalanced = alt->parent;
                notBalanced->balance += 1;
            }
        }


        if (toDelete->parent == nullptr) {
            _root = alt;
        } else {
            if (toDelete->parent->left == toDelete)
                toDelete->parent->left = alt;
            else
                toDelete->parent->right = alt;
        }
        if (alt != nullptr) {
            alt->parent = toDelete->parent;
        }

        toDelete->right = nullptr;
        toDelete->left = nullptr;
        delete toDelete;
        --_size;

        // fix balance
        while (notBalanced) {
            switch (notBalanced->balance) {
                case 2:
                    if (notBalanced->right->balance == -1)
                        notBalanced = double_left_rotate(notBalanced);
                    else
                        notBalanced = left_rotate(notBalanced);
                    break;
                case -2:
                    if (notBalanced->left->balance == 1)
                        notBalanced = double_right_rotate(notBalanced);
                    else
                        notBalanced = right_rotate(notBalanced);
                    break;
            }
            if (notBalanced->balance == 1 || notBalanced->balance == -1)
                break;
            if (notBalanced->parent) {
                if (notBalanced->parent->left == notBalanced)
                    notBalanced->parent->balance += 1;
                else
                    notBalanced->parent->balance -= 1;
            }
            notBalanced = notBalanced->parent;
        }
        return 1;
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::iterator map<Key, T, Compare>::find(const Key &key) {
        Node *now = _root;
        while (now) {
            if (_comp(now->value.first, key)) {
                now = now->right;
            } else if (_comp(key, now->value.first)) {
                now = now->left;
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

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::Node *map<Key, T, Compare>::double_left_rotate(map::Node *p) {
        if (p->balance != 2) return p;
        Node *c = p->right;
        if (c->balance != -1) return p;
        Node *b = c->left;

        p->right = b->left;
        if (b->left) b->left->parent = p;
        c->left = b->right;
        if (b->right) b->right->parent = c;

        b->parent = p->parent;
        if (p->parent) {
            if (p->parent->left == p)
                p->parent->left = b;
            else
                p->parent->right = b;
        } else {
            _root = b;
        }

        b->left = p;
        p->parent = b;
        b->right = c;
        c->parent = b;

        if (b->balance == 0) {
            p->balance = 0;
            c->balance = 0;
        } else if (b->balance == 1) {
            p->balance = -1;
            c->balance = 0;
        } else {
            p->balance = 0;
            c->balance = 1;
        }
        b->balance = 0;
        return b;
    }

    template<typename Key, typename T, typename Compare>
    typename map<Key, T, Compare>::Node *map<Key, T, Compare>::double_right_rotate(map::Node *p) {
        if (p->balance != -2) return p;
        Node *a = p->left;
        if (a->balance != 1) return p;
        Node *b = a->right;

        a->right = b->left;
        if (b->left) b->left->parent = a;
        p->left = b->right;
        if (b->right) b->right->parent = p;

        b->parent = p->parent;
        if (p->parent) {
            if (p->parent->left == p)
                p->parent->left = b;
            else
                p->parent->right = b;
        } else {
            _root = b;
        }

        b->left = a;
        a->parent = b;
        b->right = p;
        p->parent = b;

        if (b->balance == 0) {
            a->balance = 0;
            p->balance = 0;
        } else if (b->balance == 1) {
            a->balance = -1;
            p->balance = 0;
        } else {
            a->balance = 0;
            p->balance = 1;
        }
        b->balance = 0;

        return b;
    }

} // mstd

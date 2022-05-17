#pragma once

#include <iostream>
#include "functional.h"

namespace mstd {

    //--------------------------------
    // string
    //--------------------------------
    class string {
        size_t _len;
        size_t _cap;
        char *_data;

    public:
        //--------------------------------
        // Constructors and destructor
        //--------------------------------
        string(const char *s = "");

        string(size_t n, char c);

        ~string();

        //--------------------------------
        // Copy and move constructors
        //--------------------------------
        string(const string &s);

        string(string &&s) noexcept;

        //--------------------------------
        // Copy and move assignment
        //--------------------------------
        string &operator=(const string &s);

        string &operator=(string &&s) noexcept;

        //--------------------------------
        // Element access
        //--------------------------------
        char &operator[](size_t i);

        const char &operator[](size_t i) const;

        char &front();

        [[nodiscard]] const char &front() const;

        char &back();

        [[nodiscard]] const char &back() const;

        const char *c_str() const;

        //--------------------------------
        // Iterators
        //--------------------------------
    private:
        template<typename P>
        class iterator_template {
            char *_curr;

            iterator_template(char *curr) : _curr(curr) {}

        public:
            inline P &operator*() const { return *_curr; }

            inline P *operator->() const { return _curr; }

            friend inline bool operator==(const iterator_template &lhs, const iterator_template &rhs) {
                return lhs._curr == rhs._curr;
            }

            friend inline bool operator!=(const iterator_template &lhs, const iterator_template &rhs) {
                return !operator==(lhs, rhs);
            }

            inline iterator_template &operator++() {
                ++_curr;
                return *this;
            }

            inline iterator_template operator++(int) {
                iterator_template old = *this;
                operator++();
                return old;
            }

            inline iterator_template &operator--() {
                --_curr;
                return *this;
            }

            inline iterator_template operator--(int) {
                iterator_template old = *this;
                operator--();
                return old;
            }

            friend class string;
        };

        template<typename P>
        class reverse_iterator_template {
            char *_curr;

            reverse_iterator_template(char *curr) : _curr(curr) {}

        public:
            inline P &operator*() const { return *_curr; }

            inline P *operator->() const { return _curr; }

            friend inline bool operator==(const reverse_iterator_template &lhs, const reverse_iterator_template &rhs) {
                return lhs._curr == rhs._curr;
            }

            friend inline bool operator!=(const reverse_iterator_template &lhs, const reverse_iterator_template &rhs) {
                return !operator==(lhs, rhs);
            }

            inline reverse_iterator_template &operator++() {
                --_curr;
                return *this;
            }

            inline reverse_iterator_template operator++(int) {
                reverse_iterator_template old = *this;
                operator++();
                return old;
            }

            inline reverse_iterator_template &operator--() {
                ++_curr;
                return *this;
            }

            inline reverse_iterator_template operator--(int) {
                reverse_iterator_template old = *this;
                operator--();
                return old;
            }

            friend class string;
        };

    public:
        using iterator = iterator_template<char>;
        using const_iterator = iterator_template<const char>;
        using reverse_iterator = reverse_iterator_template<char>;
        using const_reverse_iterator = reverse_iterator_template<const char>;

        iterator begin();

        iterator end();

        [[nodiscard]] const_iterator cbegin() const;

        [[nodiscard]] const_iterator cend() const;

        reverse_iterator rbegin();

        reverse_iterator rend();

        [[nodiscard]] const_reverse_iterator crbegin() const;

        const_reverse_iterator crend() const;

        //--------------------------------
        // Capacity
        //--------------------------------
        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        void reserve(size_t new_cap);

        [[nodiscard]] size_t capacity() const;

        void shrink_to_fit();

    private:
        void reserve_no_check(size_t new_cap);

    public:
        //--------------------------------
        // Operations
        //--------------------------------
        void clear();

        void push_back(char c);

        void pop_back();

        string &operator+=(const string &s);

        inline string &operator+=(char ch) {
            push_back(ch);
            return *this;
        }

        string &operator+=(const char *s);

        void resize(size_t count, char ch = char());

        //--------------------------------
        // Non-member functions
        //--------------------------------
        friend inline string operator+(string lhs, const string &rhs) {
            lhs += rhs;
            return lhs;
        }

        friend inline string operator+(string lhs, char rhs) {
            lhs += rhs;
            return lhs;
        }

        friend inline string operator+(string lhs, const char *rhs) {
            lhs += rhs;
            return lhs;
        }

        friend bool operator==(const string &lhs, const string &rhs);

        friend inline bool operator!=(const string &lhs, const string &rhs) { return !(lhs == rhs); }

        friend bool operator<(const string &lhs, const string &rhs);

        friend inline bool operator>(const string &lhs, const string &rhs) { return rhs < lhs; }

        friend inline bool operator<=(const string &lhs, const string &rhs) { return !(lhs > rhs); }

        friend inline bool operator>=(const string &lhs, const string &rhs) { return !(lhs < rhs); }
    };

} // mstd

#include "./string.h"
#include "cstring.h"

namespace mstd {

    string::string(const char *s) : _len(0), _cap(0), _data(nullptr) {
        _len = mstd::strlen(s);
        _cap = _len + 1;
        _data = new char[_cap];
        mstd::strcpy(_data, s);
    }

    string::string(size_t n, char c) : _len(n), _cap(n + 1), _data(nullptr) {
        _data = new char[_cap];
        for (char *curr = _data; curr < _data + _len; ++curr) {
            *curr = c;
        }
    }

    string::~string() {
        delete[] _data;
    }

    string::string(const string &s) : _len(s._len), _cap(s._cap), _data(nullptr) {
        _data = new char[_cap];
        mstd::strcpy(_data, s._data);
    }

    string::string(string &&s) noexcept : _len(s._len), _cap(s._cap), _data(s._data) {
        s._data = nullptr;
    }

    string &string::operator=(const string &s) {
        if (this == &s) return *this;
        _len = s._len;
        _cap = s._cap;
        _data = new char[_cap];
        mstd::strcpy(_data, s._data);
        return *this;
    }

    string &string::operator=(string &&s) noexcept {
        if (this == &s) return *this;
        _len = s._len;
        _cap = s._cap;
        delete[] _data;
        _data = s._data;
        s._data = nullptr;
        return *this;
    }

    char &string::operator[](size_t i) {
        return _data[i];
    }

    const char &string::operator[](size_t i) const {
        return _data[i];
    }

    char &string::front() {
        return _data[0];
    }

    const char &string::front() const {
        return _data[0];
    }

    char &string::back() {
        return _data[_len - 1];
    }

    const char &string::back() const {
        return _data[_len - 1];
    }

    const char *string::c_str() const {
        return _data;
    }

    string::iterator string::begin() {
        return {_data};
    }

    string::iterator string::end() {
        return {_data + _len};
    }

    string::const_iterator string::cbegin() const {
        return {_data};
    }

    string::const_iterator string::cend() const {
        return {_data + _len};
    }

    string::reverse_iterator string::rbegin() {
        return {_data + _len - 1};
    }

    string::reverse_iterator string::rend() {
        return {_data - 1};
    }

    string::const_reverse_iterator string::crbegin() const {
        return {_data + _len - 1};
    }

    string::const_reverse_iterator string::crend() const {
        return {_data - 1};
    }

    bool string::empty() const {
        return _len == 0;
    }

    size_t string::size() const {
        return _len;
    }

    void string::reserve(size_t new_cap) {
        if (new_cap <= _cap) return;
        reserve_no_check(new_cap);
    }

    size_t string::capacity() const {
        return _cap;
    }

    void string::shrink_to_fit() {
        if (_len + 1 >= _cap) return;
        reserve_no_check(_len + 1);
    }

    void string::reserve_no_check(size_t new_cap) {
        _cap = new_cap;
        char *temp = new char[_cap];
        mstd::strcpy(temp, _data);
        delete[] _data;
        _data = temp;
    }

    void string::clear() {
        delete[] _data;
        _len = 0;
        _cap = 1;
        _data = new char[1];
        _data[0] = 0;
    }

    void string::push_back(char c) {
        if (_len + 1 >= _cap) {
            reserve_no_check((_cap << 1) + 1);
        }
        _data[_len] = c;
        _data[_len + 1] = 0;
        ++_len;
    }

    void string::pop_back() {
        _data[_len - 1] = 0;
        --_len;
    }

    string &string::operator+=(const string &s) {
        if (_len + s._len + 1 > _cap) {
            reserve_no_check(_len + s._len + 1);
        }
        mstd::strcpy(_data + _len, s._data);
        _len += s._len;
        return *this;
    }

    string &string::operator+=(const char *s) {
        size_t s_len = strlen(s);
        if (_len + s_len + 1 > _cap) {
            reserve_no_check(_len + s_len + 1);
        }
        mstd::strcpy(_data + _len, s);
        _len += s_len;
        return *this;
    }

    void string::resize(size_t count, char ch) {
        reserve(count);
        for (char *curr = _data + _len; curr < _data + count; ++curr) {
            *curr = ch;
        }
    }

    bool operator==(const string &lhs, const string &rhs) {
        return mstd::strcmp(lhs._data, rhs._data) == 0;
    }

    bool operator<(const string &lhs, const string &rhs) {
        return mstd::strcmp(lhs._data, rhs._data) < 0;
    }

} // mstd

#pragma once

namespace mstd {

    template<typename T1, typename T2>
    struct pair {
        T1 first;
        T2 second;
    };

    template<typename T1, typename T2>
    pair<T1, T2> make_pair(const T1 &first, const T1 &second) {
        return {first, second};
    }

    template<typename T>
    void swap(T &a, T &b) {
        T temp = a;
        a = b;
        b = temp;
    }

} // mstd
#pragma once

namespace mstd {

    template<typename T>
    struct less {
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
    };

} // mstd
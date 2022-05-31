#pragma once

#include "AlgorithmInterface.h"
#include "../lib/vector.h"
#include "../lib/bitstream.h"

using mstd::vector;

namespace Compress {

    class LZW final : public AlgorithmInterface {
        using code_t = uint64_t;
        using byte_sequence = vector<char>;

        struct ByteSequenceCompare {
            bool operator()(const byte_sequence &lhs, const byte_sequence &rhs) const {
                if (lhs.size() != rhs.size()) {
                    return lhs.size() < rhs.size();
                }
                for (size_t i = 0; i < lhs.size(); i++) {
                    if (lhs[i] != rhs[i]) {
                        return lhs[i] < rhs[i];
                    }
                }
                return false;
            }
        };

    public:
        void compress(std::ifstream &in, std::ofstream &out);

        void decompress(std::ifstream &in, std::ofstream &out);

    private:
        static code_t readCode(ibitstream &bs, unsigned char meaningBits);

        static void writeCode(obitstream &bs, code_t code, unsigned char meaningBits);

        static void writeByteSequence(obitstream &bs, const byte_sequence &bytes);

        static void writeByteSequence(std::ostream &os, const byte_sequence &bytes);
    };

} // Compress

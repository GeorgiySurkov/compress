#pragma once

#include "AlgorithmInterface.h"
#include "../lib/vector.h"
#include "../lib/bitstream.h"

using mstd::vector;

namespace Compress {

    class LZW final : public AlgorithmInterface {
        using code_t = uint64_t;

    public:
        void compress(std::ifstream &in, std::ofstream &out);

        void decompress(std::ifstream &in, std::ofstream &out);

    private:
        static code_t readCode(ibitstream &bs, unsigned char meaningBits);

        static void writeCode(obitstream &bs, code_t code, unsigned char meaningBits);
    };

} // Compress

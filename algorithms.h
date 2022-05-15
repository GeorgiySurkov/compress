#pragma once

#include "algorithms/AlgorithmInterface.h"
#include "algorithms/Huffman.h"

namespace Compress {

    enum class AlgorithmType {
        HUFFMAN,
//        LZW
    };

    void decompress(std::ifstream &in, std::ofstream &out);

    void compress(const char *method, std::ifstream &in, std::ofstream &out);

} // Compress

#pragma once

#include <fstream>

namespace Compress {

    class AlgorithmInterface {
    public:
        virtual ~AlgorithmInterface() = default;

        virtual void compress(std::ifstream &in, std::ofstream &out) = 0;

        virtual void decompress(std::ifstream &in, std::ofstream &out) = 0;
    };

} // Compress

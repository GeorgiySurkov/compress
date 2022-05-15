#include "algorithms.h"

#include "lib/cstring.h"

namespace Compress {

    void decompress(std::ifstream &in, std::ofstream &out) {
        uint8_t algorithmType;
        in >> algorithmType;
        AlgorithmInterface *algorithm;

        switch (static_cast<AlgorithmType>(algorithmType)) {
            case AlgorithmType::HUFFMAN:
                algorithm = new Huffman();
        }

        algorithm->decompress(in, out);
        delete algorithm;
    }

    void compress(const char *method, std::ifstream &in, std::ofstream &out) {
        AlgorithmType type;
        AlgorithmInterface *algorithm;

        if (mstd::strcmp(method, "huffman") == 0) {
            type = AlgorithmType::HUFFMAN;
            algorithm = new Huffman();
        } else {
            throw std::invalid_argument("Invalid compress method.");
        }

        auto castedType = (uint8_t) type;
        out.write((char *) &castedType, sizeof(castedType));
        algorithm->compress(in, out);
        delete algorithm;
    }

} // Compress
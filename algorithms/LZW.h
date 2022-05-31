#pragma once

#include "AlgorithmInterface.h"
#include "../lib/vector.h"
#include "../lib/bitstream.h"

using mstd::vector;

namespace Compress {

    class LZW final : public AlgorithmInterface {
        using code_t = int64_t;

        struct LzwSequence {
            code_t prevSequenceCode;
            code_t endingChar;
        };

        struct LzwSequenceCompare {
            bool operator()(const LzwSequence &code1, const LzwSequence &code2) const {
                if (code1.prevSequenceCode != code2.prevSequenceCode) {
                    return code1.prevSequenceCode < code2.prevSequenceCode;
                }
                return code1.endingChar < code2.endingChar;
            };
        };

    public:
        void compress(std::ifstream &in, std::ofstream &out);

        void decompress(std::ifstream &in, std::ofstream &out);

    private:
        static code_t readCode(ibitstream &bs, unsigned char meaningBits);

        static void writeCode(obitstream &bs, code_t code, unsigned char meaningBits);

        static void writeSequenceByCode(vector<LzwSequence> &table, code_t code, std::ofstream &out);

        static code_t getFirstCharOfSequence(vector<LzwSequence> &table, code_t code);
    };

} // Compress

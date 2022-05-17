#pragma once

#include "AlgorithmInterface.h"
#include "../lib/bitstream.h"
#include "../lib/avl_map.h"
#include "../lib/vector.h"

using mstd::map;
using mstd::vector;

namespace Compress {

    class Huffman final : public AlgorithmInterface {
        using symbol_t = int16_t;

        struct Node {
            Node *zero;
            Node *one;
            symbol_t symbol;

            // constructor for leaf nodes
            Node(symbol_t symbol) : zero(nullptr), one(nullptr), symbol(symbol) {}

            // constructor for not-leaf nodes
            Node(Node *zero, Node *one) : zero(zero), one(one) {}

            ~Node() {
                delete zero;
                delete one;
            }
        };

    public:
        void compress(std::ifstream &in, std::ofstream &out);

        void decompress(std::ifstream &in, std::ofstream &out);

    private:
        static void saveTree(Node *node, obitstream &os);

        static Node *readTree(ibitstream &is);

        // TODO: use bitmap instead of vector of bools for memory efficiency
        static map<symbol_t, vector<bool>> traverseTree(Node *node);

        static void _traverseTreeHelper(Node *node, vector<bool> &vec, map<symbol_t, vector<bool>> &result);
    };

} // Compress

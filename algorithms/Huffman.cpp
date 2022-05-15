#include "Huffman.h"

namespace Compress {

    void Huffman::compress(std::ifstream &in, std::ofstream &out) {
        auto dataBegin = in.tellg();

        // Count symbols
        map<symbol_t, unsigned long long> symbolAmount;
        char currChar;
        while (in.get(currChar)) {
            auto it = symbolAmount.find(currChar);
            if (it == symbolAmount.end()) {
                symbolAmount[currChar] = 1;
            } else {
                ++(it->second);
            }
        }
        symbolAmount[-1] = 1; // -1 represents EOF symbol;

        // Build tree
        struct Aboba {
            Node *treeNode;
            unsigned long long amount;
        };
        vector<Aboba> symbols;
        symbols.reserve((symbolAmount.size()));
        for (auto it = symbolAmount.rbegin(); it != symbolAmount.rend(); ++it) {
            symbols.push_back({new Node{it->first}, it->second});
        }
        while (symbols.size() > 1) {
            Node *zero, *one;
            zero = symbols[symbols.size() - 2].treeNode;
            one = symbols[symbols.size() - 1].treeNode;
            symbols[symbols.size() - 2].treeNode = new Node{zero, one};
            symbols[symbols.size() - 2].amount += symbols[symbols.size() - 1].amount;
            symbols.pop_back();
            for (size_t i = symbols.size() - 1; i > 0 && symbols[i - 1].amount <= symbols[i].amount; --i) {
                mstd::swap(symbols[i - 1], symbols[i]);
            }
        }

        // Writing Huffman codes tree to file
        Node *root = symbols.front().treeNode;
        obitstream obs{out};
        saveTree(root, obs);

        // Encoding data
        map<symbol_t, vector<bool>> symbol2code = traverseTree(root);
        delete root;
        in.clear();
        in.seekg(dataBegin);
        while (in.get(currChar)) {
            for (auto bit : symbol2code[currChar]) {
                obs.putBit(bit);
            }
        }
        // write EOF code
        for (auto bit : symbol2code[-1]) {
            obs.putBit(bit);
        }
    }

    void Huffman::decompress(std::ifstream &in, std::ofstream &out) {
        ibitstream ibs{in};

        // Read Huffman codes tree from file
        Node *root = readTree(ibs);

        // Decoding data
        Node *currNode = root;
        while (!ibs.eof()) {
            if (ibs.getBit() == 1) {
                currNode = currNode->one;
            } else {
                currNode = currNode->zero;
            }
            if (currNode->zero == nullptr && currNode->one == nullptr) { // is leaf node
                if (currNode->symbol == -1) {
                    break;
                } else {
                    out.put((char) currNode->symbol);
                    currNode = root;
                }
            }
        }
        delete root;
    }

    void Huffman::saveTree(Node *node, obitstream &os) {
        // TODO: check that we can write
        if (node->zero == nullptr && node->one == nullptr) {
            os.putBit(true);
            os.putNBytes((unsigned char *) &(node->symbol), sizeof(node->symbol));
        } else {
            os.putBit(false);
            saveTree(node->zero, os);
            saveTree(node->one, os);
        }
    }

    Huffman::Node *Huffman::readTree(ibitstream &is) {
        // TODO: check that we can read
        int isLeafNode = is.getBit();
        if (isLeafNode == 1) {
            symbol_t symbol;
            is.getNBytes((unsigned char *) &symbol, sizeof(symbol));
            return new Node{symbol};
        } else {
            Node *zero, *one;
            zero = readTree(is);
            one = readTree(is);
            return new Node{zero, one};
        }
    }

    map<Huffman::symbol_t, vector<bool>> Huffman::traverseTree(Node *node) {
        vector<bool> vec;
        map<Huffman::symbol_t, vector<bool>> res;
        _traverseTreeHelper(node, vec, res);
        return res;
    }

    void Huffman::_traverseTreeHelper(
            Node *node,
            vector<bool> &vec,
            map<symbol_t, vector<bool>> &result
    ) {
        if (node->zero == nullptr && node->one == nullptr) { // leaf node
            result[node->symbol] = vec;
        } else { // not-leaf node
            vec.push_back(false);
            _traverseTreeHelper(node->zero, vec, result);
            vec.pop_back();
            vec.push_back(true);
            _traverseTreeHelper(node->one, vec, result);
            vec.pop_back();
        }
    }

} // Compress
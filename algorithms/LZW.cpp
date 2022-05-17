#include "LZW.h"
#include "../lib/string.h"
#include "../lib/avl_map.h"

using mstd::map;
using mstd::string;

namespace Compress {

    void LZW::compress(std::ifstream &in, std::ofstream &out) {
        // Initialize table
        map<string, code_t> table;
        code_t lastCode;
        unsigned char meaningBits = 8;
        for (lastCode = 0; lastCode <= 0xFF; lastCode++) {
            table[string(1, (char) lastCode)] = lastCode;
        }

        // Encode data
        obitstream bs(out);
        char currChar;
        string currString;
        while (in.get(currChar)) {
            string newString = currString;
            newString += currChar;
            auto it = table.find(newString);
            if (it != table.end()) {
                currString = std::move(newString);
            } else {
                writeCode(bs, table[currString], meaningBits);
                table[newString] = ++lastCode;
                if (table.size() > (1 << meaningBits)) {
                    ++meaningBits;
                }
                if (meaningBits > 64) {
                    throw std::runtime_error("Bad file");
                }
                currString = string(1, currChar);
            }
        }
        writeCode(bs, table[currString], meaningBits);
    }

    void LZW::decompress(std::ifstream &in, std::ofstream &out) {
        // Initialize table
        map<string, code_t> table;
        map<code_t, string> codeTable;
        code_t lastCode;
        unsigned char meaningBits = 8;
        for (lastCode = 0; lastCode <= 0xFF; lastCode++) {
            codeTable[lastCode] = string(1, (char) lastCode);
            table[string(1, (char) lastCode)] = lastCode;
        }

        // Decode data
        ibitstream bs(in);
        code_t currCode = readCode(bs, meaningBits);
        if (in.bad()) {
            return;
        }
        auto currCodeIt = codeTable.find(currCode);
        if (currCodeIt == codeTable.end()) {
            throw std::runtime_error("Bad file");
        }
        string currString = currCodeIt->second, newString;
        ++meaningBits; // Here we know exactly that there are new phrases in table, so we should increment code size.
        auto prevCodeIt = currCodeIt;
        currCode = readCode(bs, meaningBits);
        currCodeIt = codeTable.find(currCode);
        if (currCodeIt == codeTable.end()) {
            newString = prevCodeIt->second + prevCodeIt->second[0];
            currCodeIt = codeTable.insert({currCode, newString}).first;
        }
        while (in.good()) {
            newString = currString + currCodeIt->second[0];
            auto newStringIt = table.find(newString);
            if (newStringIt != table.end()) {
                currString = std::move(newString);
            } else {
                currString = currCodeIt->second;
                out.write(prevCodeIt->second.c_str(), (long) prevCodeIt->second.size());

                // Add new code to table
                ++lastCode;
                codeTable[lastCode] = newString;
                table[newString] = lastCode;
                if (table.size() >= (1 << meaningBits)) {
                    ++meaningBits;
                }
                if (meaningBits > 32) {
                    throw std::runtime_error("Bad file");
                }
            }
            prevCodeIt = currCodeIt;
            currCode = readCode(bs, meaningBits);
            currCodeIt = codeTable.find(currCode);
            if (currCodeIt == codeTable.end()) {
                newString = prevCodeIt->second + prevCodeIt->second[0];
                currCodeIt = codeTable.insert({currCode, newString}).first;
            }
        }
        out.write(prevCodeIt->second.c_str(), (long) prevCodeIt->second.size());
    }

    LZW::code_t LZW::readCode(ibitstream &bs, unsigned char meaningBits) {
        code_t result = 0;
        for (long i = meaningBits - 1; i >= 0; --i) {
            result |= bs.getBit() << i;
        }
        return result;
    }

    void LZW::writeCode(obitstream &bs, code_t code, unsigned char meaningBits) {
        for (long i = meaningBits - 1; i >= 0; --i) {
            bs.putBit(((code >> i) & 1) == 1);
        }
    }


} // Compress
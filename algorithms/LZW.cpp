#include "LZW.h"
#include "../lib/avl_map.h"

using mstd::map;

namespace Compress {

    void LZW::compress(std::ifstream &in, std::ofstream &out) {
        map<LzwSequence, code_t, LzwSequenceCompare> table;
        code_t lastCode = 0xFF;
        unsigned char meaningBits = 9;
        unsigned maxCode = 1 << meaningBits;
        obitstream bs(out);
        LzwSequence curr;
        curr.prevSequenceCode = in.get();
        if (curr.prevSequenceCode == EOF) {
            return;
        }
        while ((curr.endingChar = in.get()) != EOF) {
            auto p = table.find(curr);
            while (p != table.end()) {
                curr.prevSequenceCode = p->second;
                if ((curr.endingChar = in.get()) == EOF) {
                    writeCode(bs, curr.prevSequenceCode, meaningBits);
                    return;
                }
                p = table.find(curr);
            }
            table[curr] = ++lastCode;
            if (lastCode >= maxCode - 1) {
                ++meaningBits;
                maxCode <<= 1;
            }
            if (meaningBits > 63) { // TODO: add support for files that need more than 63 bits for code size
                throw std::runtime_error("Can't handle file");
            }
            writeCode(bs, curr.prevSequenceCode, meaningBits);
            curr.prevSequenceCode = curr.endingChar;
        }
        writeCode(bs, curr.prevSequenceCode, meaningBits);
    }

    void LZW::decompress(std::ifstream &in, std::ofstream &out) {
        vector<LzwSequence> table;
        unsigned char meaningBits = 9;
        unsigned maxCode = 1 << meaningBits;
        ibitstream bs(in);
        code_t c1 = readCode(bs, meaningBits), c2 = readCode(bs, meaningBits);
        while (in.good()) {
            writeSequenceByCode(table, c1, out);
            table.push_back((c2 - 0xFF >= (code_t) table.size() + 1)
                            ? LzwSequence{c1, getFirstCharOfSequence(table, c1)}
                            : LzwSequence{c1, getFirstCharOfSequence(table, c2)}
            );
            c1 = c2;
            if (table.size() + 0xFF >= maxCode - 3) {
                ++meaningBits;
                maxCode <<= 1;
            }
            if (meaningBits > 63) { // TODO: add support for files that need more than 63 bits for code size
                throw std::runtime_error("Can't handle file");
            }
            c2 = readCode(bs, meaningBits);
        }
        writeSequenceByCode(table, c1, out);
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

    void LZW::writeSequenceByCode(vector<LzwSequence> &table, code_t code, std::ofstream &out) {
        if (code <= 0xFF) {
            out.put((char) code);
            return;
        }
        code -= 256;
        if (table[code].prevSequenceCode > 0xFF) {
            writeSequenceByCode(table, table[code].prevSequenceCode, out);
        } else {
            out.put((char) table[code].prevSequenceCode);
        }
        out.put((char) table[code].endingChar);
    }

    LZW::code_t LZW::getFirstCharOfSequence(vector<LzwSequence> &table, LZW::code_t code) {
        if (code <= 0xFF) {
            return code;
        }
        code -= 256;
        if (table[code].prevSequenceCode > 0xFF) {
            return getFirstCharOfSequence(table, table[code].prevSequenceCode);
        }
        return table[code].prevSequenceCode;
    }

} // Compress
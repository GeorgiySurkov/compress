#pragma once

#include <fstream>

namespace Compress {

    //--------------------------------
    // ibitstream
    //--------------------------------
    class ibitstream {
        std::istream &_is;
        unsigned char _buffer;
        unsigned char _bitCount;

    public:
        explicit ibitstream(std::istream &is) :
                _bitCount(0),
                _is(is) {}

        int getBit() {
            if (_bitCount == 8) _bitCount = 0;
            if (_bitCount == 0) _buffer = _is.get(); // std::ifstream::get()
            int i = (_buffer & (1 << (7 - _bitCount))) ? 1 : 0;
            ++_bitCount;
            return i;
        }

        void getNBytes(unsigned char *ptr, std::streamsize n) {
            if (_bitCount == 0) {
                _is.read((char *) ptr, n);
            } else {
                unsigned char temp;
                for (unsigned char *curr = ptr; curr < ptr + n; ++curr) {
                    temp = _buffer << _bitCount;
                    _buffer = _is.get();
                    temp |= _buffer >> (8 - _bitCount);
                    *curr = temp;
                }
            }
        }

        [[nodiscard]] bool eof() const {
            if (_bitCount > 0 && _bitCount < 8) return false;
            if (_is.peek() != EOF) return false;
            return true;
        }
    };

    //--------------------------------
    // obitstream
    //--------------------------------
    class obitstream {
        std::ostream &_os;
        unsigned char _buffer;
        unsigned char _bitCount;

    public:
        explicit obitstream(std::ostream &os) :
                _buffer(0),
                _bitCount(0),
                _os(os) {}

        ~obitstream() {
            if (_bitCount > 0)
                _os.put(_buffer);
        }

        void putBit(bool x) {
            unsigned char S = x;
            _buffer = _buffer | (S << (7 - _bitCount));
            ++_bitCount;
            if (_bitCount == 8) {
                _os.put(_buffer);
                _bitCount = 0;
                _buffer = 0;
            }
        }

//        void putByte(char x) {
//            if (_bitCount == 0) {
//                _os.put(x);
//            } else {
//                _buffer |= x >> _bitCount;
//                _os.put(_buffer);
//                _buffer = x << (8 - _bitCount);
//            }
//        }

        void putNBytes(const unsigned char *ptr, std::streamsize n) {
            if (_bitCount == 0) {
                _os.write((const char *) ptr, n);
            } else {
                for (const unsigned char *curr = ptr; curr < ptr + n; ++curr) {
                    _buffer |= *curr >> _bitCount;
                    _os.put(_buffer);
                    _buffer = *curr << (8 - _bitCount);
                }
            }
        }

    };

} // Compress

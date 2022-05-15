#include "cstring.h"

namespace mstd {

    int strcmp(const char *s1, const char *s2) {
        while (*s1) {
            if (*s1 != *s2) {
                break;
            }

            s1++;
            s2++;
        }

        return *(const unsigned char *) s1 - *(const unsigned char *) s2;
    }

    int strncmp(const char *s1, const char *s2, size_t n) {
        const char *end = s1 + n;
        int result = 0;
        for (; result == 0 && s1 != end && (*s1 || *s2); result = *(s1++) - *(s2++));
        return result;
    }

    unsigned int strlen(const char *s) {
        unsigned int count = 0;
        while (*s != '\0') {
            count++;
            s++;
        }
        return count;
    }

    char *strcpy(char *dst, const char *src) {
        if (dst == nullptr) {
            return nullptr;
        }
        char *ptr = dst;
        while (*src != '\0') {
            *dst = *src;
            dst++;
            src++;
        }
        *dst = '\0';
        return ptr;
    }

} // mstd

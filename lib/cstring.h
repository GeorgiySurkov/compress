#pragma once

#include <iostream>

namespace mstd {

    int strcmp(const char *s1, const char *s2);

    int strncmp(const char* s1, const char* s2, size_t n);

    unsigned int strlen(const char *s);

    char* strcpy(char* dst, const char* src);

}

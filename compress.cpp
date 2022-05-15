#include <fstream>
#include <iostream>
#include "lib/cstring.h"
#include "algorithms.h"

using Compress::compress;
using Compress::decompress;

void printBeautifulName() {
    std::cout << "              ______\n"
                 "             / ____/____   ____ ___   ____   _____ ___   _____ _____\n"
                 "            / /    / __ \\ / __ `__ \\ / __ \\ / ___// _ \\ / ___// ___/\n"
                 "           / /___ / /_/ // / / / / // /_/ // /   /  __/(__  )(__  )\n"
                 "           \\____/ \\____//_/ /_/ /_// .___//_/    \\___//____//____/\n"
                 "                                  /_/"
              << std::endl;
}

void printHelpMessage() {
    printBeautifulName();
    std::cout << std::endl;
    std::cout << "Compress - simple file compression utility.\n"
                 "    -h, ?                   - view this help message.\n"
                 "\n"
                 "    -lzw input output       - compress input using lzw method and\n"
                 "                              save result to output.\n"
                 "\n"
                 "    -huffman input output   - compress input using Huffman method\n"
                 "                              and save result to output.\n"
                 "\n"
                 "    -d input output         - decompress file compressed with any\n"
                 "                              method and save result to output."
              << std::endl;
}

void printErrorMessage(const char *message) {
    std::cerr << "compress: " << message << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printErrorMessage(
                "Not enough arguments.\n"
                "type 'compress -h' to view help message."
        );
        return 1;
    }
    if (mstd::strcmp(argv[1], "-h") == 0 || mstd::strcmp(argv[1], "?") == 0) {
        if (argc != 2) {
            printErrorMessage(
                    "Wrong arguments\n"
                    "type 'compress -h' to view help message."
            );
            return 1;
        }
        printHelpMessage();
    } else if (mstd::strcmp(argv[1], "-d") == 0) {
        if (argc != 4) {
            printErrorMessage(
                    "Wrong arguments\n"
                    "type 'compress -h' to view help message."
            );
            return 1;
        }

        std::ifstream in(argv[2], std::ios::binary);
        std::ofstream out(argv[3], std::ios::binary);
        if (in.fail() || out.fail()) {
            printErrorMessage("Error while opening file"); // TODO: add more meaningful error message
        }

        decompress(in, out);
    } else if (mstd::strncmp(argv[1], "-", 1) == 0 && argc == 4) {

        std::ifstream in(argv[2], std::ios::binary);
        std::ofstream out(argv[3], std::ios::binary);
        if (in.fail() || out.fail()) {
            printErrorMessage("Error while opening file"); // TODO: add more meaningful error message
        }

        try {
            compress(argv[1] + 1, in, out);
        } catch (const std::invalid_argument &err) {
            std::cerr << err.what() << std::endl;
            return 1;
        }
    } else {
        printErrorMessage(
                "Wrong arguments\n"
                "type 'compress -h' to view help message."
        );
        return 1;
    }
    return 0;
}

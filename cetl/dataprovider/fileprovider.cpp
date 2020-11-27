#include <cetl/dataprovider/fileProvider.hpp>
#include <string.h>
#include <cstdio>
#include <stdlib.h>

void intToChar(char a[], unsigned int n) {
  memcpy(a, &n, 4);
}

unsigned int charToInt(char c[]) {
  unsigned int n = 0;
  memcpy(&n, c, 4);
  return n;
}

fileProvider::fileProvider(Buffer * b, std::string dir) {
    filename = strdup(("/" + dir + "/cetlXXXXXXXXX").c_str());
    std::fstream s = std::fstream(filename);

    auto entries = b->getEntries();
    for (auto e: entries) {
        char kLen[4];
        char vLen[4];
        intToChar(kLen, e.k.length());
        intToChar(vLen, e.v.length());
        file << kLen << vLen << e.k << e.v;
    }
    file.seekp(0);
}

fileProvider::fileProvider(std::string _filename) {
    filename = (char *) _filename.c_str();
    std::fstream s = std::fstream(filename);
    file.seekp(0);
}

entry fileProvider::next() {
    if (file.eof()) {
        return {std::string(), std::string()};
    }
    char * kvLen = (char*)malloc(sizeof(char) * 8);
    char * kLen = (char*)malloc(sizeof(char) * 4);
    char  *vLen = (char*)malloc(sizeof(char) * 4);

    file.read(kvLen, 8);

    for (unsigned int i = 0; i < 4; i++)
    {
        kLen[i] = kvLen[i];
    }

    for (unsigned int i = 4; i < 8; i++)
    {
        vLen[i] = kvLen[i];
    }


    auto kLength = charToInt(kLen);
    auto vLength = charToInt(vLen);

    char kv[kLength+vLength];
    char k[kLength];
    char v[vLength];

    file.read(kv, kLength+vLength);

    for (unsigned int i = 0; i < kLength; i++)
    {
        k[i] = kv[i];
    }

    for (unsigned int i = kLength; i < kLength+vLength; i++)
    {
        v[i] = kv[i];
    }

    return {std::string(k), std::string(v)};
}

void fileProvider::reset() {
    std::remove(filename);
}
#include <vector>
#include <string>
#include <algorithm>
#include <vector>
#include <silkworm/common/base.hpp>
#ifndef BUFFER_H
#define BUFFER_H

struct entry {
    silkworm::ByteView k;
    silkworm::ByteView v;
};

class Buffer {
   public:
    Buffer(size_t _optimalSize);

    void put(silkworm::ByteView, silkworm::ByteView);
    void reset();

    int length();
    bool checkFlushSize();
    void sort();
    std::vector<entry>::iterator begin();
    std::vector<entry>::iterator end();

   private:

    std::vector<entry> entries;
    size_t optimalSize;
    size_t size;
};

#endif
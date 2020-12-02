#include <array>
#include <string>
#include <algorithm>
#include <silkworm/common/base.hpp>
#ifndef BUFFER_H
#define BUFFER_H
#define FIX 20000000

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
    std::map<silkworm::ByteView, silkworm::ByteView>::iterator begin();
    std::map<silkworm::ByteView, silkworm::ByteView>::iterator end();

   private:

    std::map<silkworm::ByteView, silkworm::ByteView> entries;
    size_t optimalSize;
    size_t size;
};

#endif
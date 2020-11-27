#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <silkworm/common/base.hpp>
#ifndef BUFFER_H
#define BUFFER_H

struct entry {
    silkworm::ByteView k;
    silkworm::ByteView v;
};

class Buffer {
   public:

    virtual void put(silkworm::ByteView, silkworm::ByteView) = 0;
    virtual void reset() = 0;
    virtual void sort() = 0;

    virtual int length() = 0;
    virtual bool checkFlushSize() = 0;
    virtual std::vector<entry> getEntries() = 0;
    virtual entry get(int) = 0;
};
#endif
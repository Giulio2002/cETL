#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <silkworm/common/base.hpp>
#ifndef SORTABLE_H
#define SORTABLE_H

struct entry {
    silkworm::ByteView k;
    silkworm::ByteView v;
};

class SortableBuffer {
   public:
    SortableBuffer(size_t _optimalSize);

    void put(silkworm::ByteView, silkworm::ByteView);
    void reset();
    void sort();

    int length();
    bool checkFlushSize();
    std::vector<entry> getEntries();
    entry get(int);

   private:

    std::vector<entry> entries;

    size_t optimalSize;
    size_t size;
};

#endif
#include <cetl/buffers/buffer.hpp>

#ifndef SORTABLE_H
#define SORTABLE_H

class SortableBuffer: public Buffer {
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
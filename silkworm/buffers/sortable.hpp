#include <silkworm/buffers/buffer.hpp>

class SortableBuffer: public Buffer {
   public:
    SortableBuffer(size_t _optimalSize);

    void put(std::string, std::string);
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
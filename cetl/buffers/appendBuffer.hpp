#include <cetl/buffers/buffer.hpp>

class AppendBuffer: public Buffer {
   public:
    AppendBuffer(size_t _optimalSize);

    void put(silkworm::ByteView, silkworm::ByteView);
    void reset();
    void sort();

    int length();
    bool checkFlushSize();
    std::vector<entry> getEntries();
    entry get(int);

   private:

    std::vector<entry> buffer;
    std::map<silkworm::ByteView, silkworm::ByteView> entries;
    size_t optimalSize;
    size_t size;
};
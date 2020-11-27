#include <cetl/buffers/buffer.hpp>

class AppendBuffer: public Buffer {
   public:
    AppendBuffer(size_t _optimalSize);

    void put(std::string, std::string);
    void reset();
    void sort();

    int length();
    bool checkFlushSize();
    std::vector<entry> getEntries();
    entry get(int);

   private:

    std::vector<entry> buffer;
    std::map<std::string, std::string> entries;
    size_t optimalSize;
    size_t size;
};
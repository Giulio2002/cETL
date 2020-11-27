#include <cetl/buffers/buffer.hpp>

class AppearBuffer: public Buffer {
   public:
    AppearBuffer(size_t _optimalSize);

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
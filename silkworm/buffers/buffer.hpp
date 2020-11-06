
#include <vector>
#include <string>
#include <map>
#include <algorithm>

struct entry {
    std::string k;
    std::string v;
};

class Buffer {
   public:

    virtual void put(std::string, std::string) = 0;
    virtual void reset() = 0;
    virtual void sort() = 0;

    virtual int length() = 0;
    virtual bool checkFlushSize() = 0;
    virtual std::vector<entry> getEntries() = 0;
    virtual entry get(int) = 0;
};
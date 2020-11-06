
#include <vector>
#include <string>
#include <map>
#include <algorithm>

struct entry {
    std::string k;
    std::string v;
};


bool comp(const entry lhs, const entry rhs) { return lhs.k.compare(rhs.k) < 0; }

class Buffer {
   public:

    void put(std::string, std::string);
    void reset();
    void sort();

    int length();
    bool checkFlushSize();
    std::vector<entry> getEntries();
    entry get(int);
};
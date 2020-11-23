
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <silkworm/buffers/buffer.hpp>

class DataProvider {
   public:

    virtual entry next() = 0;
    virtual void reset() = 0;
};
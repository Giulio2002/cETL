
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cetl/buffers/buffer.hpp>

#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

class DataProvider {
   public:

    virtual entry next() = 0;
    virtual void reset() = 0;
};

#endif
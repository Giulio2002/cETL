
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cetl/buffers/buffer.hpp>
#include <fstream>

#ifndef FILE_PROVIDER_H
#define FILE_PROVIDER_H

class FileProvider {
    public:

        FileProvider(Buffer *, int);
        entry next();
        void reset();

    private:
        std::fstream file;
        char * filename;
};
#endif
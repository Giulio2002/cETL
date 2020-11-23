#include <silkworm/dataprovider/dataprovider.hpp>
#include <fstream>

class fileProvider: public DataProvider {
    public:

        fileProvider(Buffer *);
        entry next();
        void reset();

    private:
        std::fstream file;
        char * filename;
};
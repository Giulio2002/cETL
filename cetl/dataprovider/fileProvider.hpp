#include <cetl/dataprovider/dataprovider.hpp>
#include <fstream>

class fileProvider: public DataProvider {
    public:

        fileProvider(Buffer *, std::string);
        fileProvider(std::string);
        entry next();
        void reset();

    private:
        std::fstream file;
        char * filename;
};
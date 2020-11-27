#include <cetl/dataprovider/dataprovider.hpp>

class memProvider: public DataProvider {
    public:

        memProvider(Buffer *);
        entry next();
        void reset();

    private:
        int pos;
        Buffer *buffer;
};
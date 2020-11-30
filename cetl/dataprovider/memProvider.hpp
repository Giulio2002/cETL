#include <cetl/dataprovider/dataprovider.hpp>

class memProvider: public DataProvider {
    public:

        memProvider(SortableBuffer *);
        entry next();
        void reset();

    private:
        int pos;
        SortableBuffer *buffer;
};
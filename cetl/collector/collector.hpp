#include <cetl/dataprovider/fileProvider.hpp>
#include <cetl/buffers/sortable.hpp>
#include <silkworm/db/chaindb.hpp>
#include <dirent.h>

typedef void (*OnLoad)(silkworm::ByteView, silkworm::ByteView);

class Collector {

    public:
        Collector(SortableBuffer*); // true = autoclean
        void flushBuffer();
        void collect(silkworm::ByteView k, silkworm::ByteView v);
        void load(silkworm::lmdb::Table *, OnLoad);
        void load(silkworm::lmdb::Table *);

    private:

	    std::vector<FileProvider *> dataProviders;
        SortableBuffer * b;
};

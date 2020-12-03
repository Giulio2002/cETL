#include <cetl/dataprovider/fileProvider.hpp>
#include <cetl/buffers/buffer.hpp>
#include <silkworm/db/chaindb.hpp>
#include <dirent.h>

typedef void (*OnLoad)(silkworm::ByteView, silkworm::ByteView);

class Collector {

    public:
        Collector(Buffer*);
        void flushBuffer();
        void collect(silkworm::ByteView k, silkworm::ByteView v);
        void load(silkworm::lmdb::Table *, OnLoad);
        void load(silkworm::lmdb::Table *);
        bool append;

    private:

	    std::vector<FileProvider *> dataProviders;
        Buffer * b;
};

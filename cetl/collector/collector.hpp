#include <cetl/dataprovider/dataprovider.hpp>
#include <cetl/buffers/sortable.hpp>
#include <silkworm/db/chaindb.hpp>
#include <dirent.h>

typedef void (*OnLoad)(silkworm::ByteView, silkworm::ByteView);

class Collector {

    public:
        Collector(std::string, bool, SortableBuffer*); // true = autoclean
        Collector(std::string); // from files
        void flushBuffer(bool);
        void collect(silkworm::ByteView k, silkworm::ByteView v);
        void load(std::unique_ptr<silkworm::lmdb::Table> t, silkworm::lmdb::Transaction * tx, OnLoad load);

    private:
        /*extractNextFunc ExtractNextFunc
	    flushBuffer     func([]byte, bool) error*/
	    std::vector<DataProvider *> dataProviders;
        SortableBuffer * b;
        std::string dir;
	    bool allFlushed;
	    bool autoclean;
};

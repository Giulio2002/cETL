#include <cetl/dataprovider/dataprovider.hpp>
#include <cetl/buffers/sortable.hpp>
#include <silkworm/db/chaindb.hpp>
#include <dirent.h>

typedef void (*OnLoad)(std::string, std::string);

class Collector {

    public:
        Collector(std::string, bool, SortableBuffer*); // true = autoclean
        Collector(std::string); // from fules
        void flushBuffer(bool);
        void collect(std::string k, std::string v);
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

#include <silkworm/dataprovider/dataprovider.hpp>
#include <silkworm/db/chaindb.hpp>

typedef OnLoad (void)(std::string, std::string);

class Collector {

    public:
        Collector(std::string, bool); // true = autoclean
        Collector(std::string); // from fules
        void flushBuffer(std::string, bool);
        void collect(std::string k, std::string v);
        void load(std::string, Table, OnLoad load, std::string start, std::string end, int fixedBits, int bufferType, int bufferSize);

    private:
        /*extractNextFunc ExtractNextFunc
	    flushBuffer     func([]byte, bool) error*/
	    std::vector<DataProvider *> dataProviders;
        Buffer * b;
	    bool allFlushed;
	    bool autoClean;
}
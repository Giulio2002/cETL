#include "api.h"
#include <cetl/collector/collector.hpp>
#include <cetl/buffers/buffer.hpp>
#include <silkworm/db/tables.hpp>
#include <silkworm/db/util.hpp>
#include <boost/endian/conversion.hpp>
#include <gsl/gsl_util>
#include <iostream>
#include <silkworm/common/log.hpp>

using namespace silkworm;

EXPORT int tg_blockhashes(MDB_txn* mdb_txn, uint64_t block_number) {
    Logger::default_logger().set_local_timezone(true);
    auto buffer = new Buffer(OPTIMAL_BUFFER_SIZE);
    auto collector = Collector(buffer);
    collector.append = true;
    lmdb::Transaction txn{/*parent=*/nullptr, mdb_txn, /*flags=*/0};
    auto cleanup{gsl::finally([&txn] { *txn.handle() = nullptr; })};
    auto from{txn.open(db::table::kBlockHeaders)};
    auto to{txn.open(db::table::kHeaderNumbers)};
    // Extract
    Bytes start(8, '\0');
    boost::endian::store_big_u64(&start[0], block_number);
    MDB_val key_mdb{db::to_mdb_val(start)};
    MDB_val data_mdb;
    SILKWORM_LOG(LogInfo) << "Started BlockHashes Extraction" << std::endl;
    int i = 0;
    for (int rc{from->seek(&key_mdb, &data_mdb)}; rc != MDB_NOTFOUND; rc = from->get_next(&key_mdb, &data_mdb)) {
        // Extraction occurs here
        ByteView k{db::from_mdb_val(key_mdb)};

        if (k.size() == 40) {
            collector.collect(k.substr(8,40), k.substr(0,8));
            if (i%100000 == 0) { SILKWORM_LOG(LogInfo) << "Collected " << i << std::endl; }
            i++;
        }
    }
    from->close();
    SILKWORM_LOG(LogInfo) << "Finished " << i << std::endl;
    SILKWORM_LOG(LogInfo) << "Started BlockHashes Loading" << std::endl;
    collector.load(to.get());
    SILKWORM_LOG(LogInfo) << "All Done" << std::endl;
    return 0;
}
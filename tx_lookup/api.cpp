#include "api.h"
#include <cetl/collector/collector.hpp>
#include <cetl/buffers/buffer.hpp>
#include <silkworm/db/tables.hpp>
#include <silkworm/db/util.hpp>
#include <boost/endian/conversion.hpp>
#include <gsl/gsl_util>
#include <iostream>
#include <silkworm/common/log.hpp>
#include <silkworm/rlp/decode.hpp>
#include <silkworm/types/block.hpp>
#include <unistd.h>
#include <silkworm/crypto/ecdsa.hpp>

using namespace silkworm;

void printByteView(ByteView b) {
    const unsigned char * ca = b.data();
    for (size_t i = 0; i < b.size(); i++)
    {
        std::cout << (int) ca[i] << " ";
    }
    std::cout << std::endl;
}

EXPORT int tg_tx_lookup(MDB_txn* mdb_txn, uint64_t) {
    Logger::default_logger().set_local_timezone(true);
    auto buffer{Buffer(OPTIMAL_BUFFER_SIZE)};
    auto collector = Collector(&buffer);
    lmdb::Transaction txn{/*parent=*/nullptr, mdb_txn, /*flags=*/0};
    auto from{txn.open(db::table::kBlockBodies)};
    auto to{txn.open(db::table::kTxLookup)};
    // Extract
    MDB_val key, data;
    //from->seek(encode_number(block_number));
    int rc = from->get_first(&key, &data);
    lmdb::err_handler(rc);
    lmdb::err_handler(from->get_next(&key, &data));
    SILKWORM_LOG(LogInfo) << "ETL: Started Tx Lookup Index Extraction [1/2]" << std::endl;
    int i = 1;
    while (rc == MDB_SUCCESS) {
        // Extraction occurs here
        std::optional<ByteView> k{db::from_mdb_val(key)};
        std::optional<ByteView> v{db::from_mdb_val(data)};

        auto block = BlockBody{};
        try {
            rlp::decode<BlockBody>(*v, block);
        } catch(...) {
            lmdb::err_handler(from->get_next(&key, &data));
            std::cout << "Skipped: ";
            printByteView(*v);
            i++;
            continue;
        }
        sleep(100);
        auto current_number{k->substr(0,8)};
        for(auto tx: block.transactions) {
            Bytes rlp{};
            ecdsa::RecoveryId x{ecdsa::get_signature_recovery_id(tx.v)};
            rlp::encode(rlp, tx, true, (uint64_t)*x.eip155_chain_id);
            ethash::hash256 hash{keccak256(rlp)};
            collector.collect(silkworm::ByteView(hash.bytes, HASH_LENGTH), current_number);
        }
        i++;
    }
    SILKWORM_LOG(LogInfo) << "ETL: Started Tx Lookup Index Loading [2/2]" << std::endl;
    collector.load(to.get());
    from->close();
    SILKWORM_LOG(LogInfo) << "ETL: All Done!" << std::endl;
    return 0;
}
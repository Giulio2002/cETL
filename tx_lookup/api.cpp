#include "api.h"
#include <cetl/collector/collector.hpp>
#include <cetl/buffers/buffer.hpp>
#include <silkworm/db/tables.hpp>
#include <silkworm/db/util.hpp>
#include <silkworm/db/access_layer.hpp>
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

EXPORT int tg_tx_lookup(MDB_txn* mdb_txn, uint64_t from_block, uint64_t to_block) {
    Logger::default_logger().set_local_timezone(true);
    auto buffer{Buffer(OPTIMAL_BUFFER_SIZE)};
    auto collector = Collector(&buffer);
    lmdb::Transaction txn{/*parent=*/nullptr, mdb_txn, /*flags=*/0};
    auto to{txn.open(db::table::kTxLookup)};
    // Extract
    SILKWORM_LOG(LogInfo) << "ETL: Started Tx Lookup Index Extraction [1/2]" << std::endl;
    for(uint64_t current = from_block; current < to_block; current++ ) {
        // Extraction occurs here
        Bytes k = Bytes(8, '\0');

        boost::endian::store_big_u64(&k[0], current);
        auto bh = db::read_block(txn, current, false);
        for(auto tx: bh->block.transactions) {
            Bytes rlp{};
            ecdsa::RecoveryId x{ecdsa::get_signature_recovery_id(tx.v)};
            rlp::encode(rlp, tx, true, (uint64_t)*x.eip155_chain_id);
            ethash::hash256 hash{keccak256(rlp)};
            collector.collect(silkworm::ByteView(hash.bytes, HASH_LENGTH), k);
        }
    }
    SILKWORM_LOG(LogInfo) << "ETL: Started Tx Lookup Index Loading [2/2]" << std::endl;
    collector.load(to.get());
    SILKWORM_LOG(LogInfo) << "ETL: All Done!" << std::endl;
    return 0;
}
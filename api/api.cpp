#include "api.h"
#include <cetl/collector/collector.hpp>
#include <cetl/buffers/sortable.hpp>
#include <silkworm/db/tables.hpp>
#include <silkworm/db/util.hpp>
#include <boost/endian/conversion.hpp>
#include <gsl/gsl_util>
#include <iostream>
#include <silkworm/common/log.hpp>

using namespace silkworm;

Bytes encode_number(uint64_t block_number) {
    uint64_t limit = 32;

    for (int bytecount = 1; bytecount <= 8; bytecount++) {
        if (block_number < limit) {
			Bytes res(bytecount, '\0');
			uint64_t b = block_number;
			for (int i = bytecount - 1; i > 0; i--) {
				res.at(i) = (char) (b & 0xff);
				b >>= 8;
			}
			res.at(0) = (char) (b | (bytecount << 5)); // 3 most significant bits of the first byte are bytecount
			return res;
		}
		limit <<= 8;
    }
    return Bytes();
}

uint64_t decode_number(Bytes encoded) {
	int bytecount = (int) (encoded.at(0) >> 5);
	uint64_t timestamp = (uint64_t)(encoded.at(0) & 0x1f);
	for (int i = 1; i < bytecount; i++) {
		timestamp = (timestamp << 8) | (uint64_t)encoded.at(i);
	}
	return timestamp;
}

/*EXPORT*/ int tg_index_account(MDB_txn* mdb_txn, uint64_t start_block) {
    auto buffer = new SortableBuffer(OPTIMAL_BUFFER_SIZE);
    auto collector = Collector(buffer);
    lmdb::Transaction txn{/*parent=*/nullptr, mdb_txn, /*flags=*/0};
    auto from{txn.open(db::table::kAccountChangeSet)};
    auto to{txn.open(db::table::kAccountHistory)};
    // Extract
    from->seek(encode_number(start_block));
    MDB_val key, data;
    int rc = from->get_current(&key, &data);
    while (rc == MDB_SUCCESS) {
        // Extraction occurs here
        ByteView k{db::from_mdb_val(key)};
        ByteView v{db::from_mdb_val(data)};
        // find n and total length
        uint32_t n = boost::endian::load_big_u32(v.substr(0, 4).data());

        if (n == 0) {
            rc = from->get_next(&key, &data);
            continue;
        }
        uint32_t offset = 4 + n*HASH_LENGTH + 4*n;

        for (uint32_t i = 0; i < n; i++)
        {
            ByteView changeset_key{v.substr(4+i*HASH_LENGTH, 4+(i+1)*HASH_LENGTH)};
            uint32_t idx_0 = 0;
            if (i > 0) {
                idx_0 = boost::endian::load_big_u32(v.substr(4+n*HASH_LENGTH+4*(i-1), 4+n*HASH_LENGTH+4*i).data());
            }
            uint32_t idx_1 = boost::endian::load_big_u32(v.substr(4+n*HASH_LENGTH+4*i, 4+n*HASH_LENGTH+4*(i+1)).data());
            ByteView changeset_value{v.substr(offset+idx_0, offset+idx_1)};
            // Collection
            uint64_t block_number = decode_number(Bytes(k.data()));
            Bytes value(9, '\0');
            boost::endian::store_big_u64(&value[0], block_number);
            if (changeset_value.size() == 0) value[8] = 1;
            else value[8] = 0;
            collector.collect(changeset_key, ByteView(value.data()));
        }
        // END of extraction
        rc = from->get_next(&key, &data);
        if (rc && rc != MDB_NOTFOUND) {
            return rc;
        }
    }
    collector.load(to.get());
    from->close();
    return txn.commit();
}

EXPORT int tg_blockhashes(MDB_txn* mdb_txn, uint64_t block_number) {
    Logger::default_logger().set_local_timezone(true);
    auto buffer = new SortableBuffer(OPTIMAL_BUFFER_SIZE);
    auto collector = Collector(buffer);
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
    std::cout << block_number << std::endl;
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
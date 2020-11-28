#include "api.h"
#include <cetl/collector/collector.hpp>
#include <cetl/buffers/sortable.hpp>
#include <silkworm/db/tables.hpp>
#include <silkworm/db/util.hpp>
#include <boost/endian/conversion.hpp>

silkworm::Bytes encode_number(uint64_t block_number) {
    uint64_t limit = 32;

    for (int bytecount = 1; bytecount <= 8; bytecount++) {
        if (block_number < limit) {
			silkworm::Bytes res(bytecount, '\0');
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
    return silkworm::Bytes();
}

uint64_t decode_number(silkworm::Bytes encoded) {
	int bytecount = (int) (encoded.at(0) >> 5);
	uint64_t timestamp = (uint64_t)(encoded.at(0) & 0x1f);
	for (int i = 1; i < bytecount; i++) {
		timestamp = (timestamp << 8) | (uint64_t)encoded.at(i);
	}
	return timestamp;
}

EXPORT int tg_index_account(MDB_txn* mdb_txn, uint64_t start_block) {
    auto buffer = new SortableBuffer(OPTIMAL_BUFFER_SIZE);
    auto collector = Collector("cetl/", false, buffer);
    silkworm::lmdb::Transaction txn{/*parent=*/nullptr, mdb_txn, /*flags=*/0};
    auto from{txn.open(silkworm::db::table::kAccountChangeSet)};
    auto to{txn.open(silkworm::db::table::kAccountHistory)};
    // Extract
    from->seek(encode_number(start_block));
    MDB_val key, data;
    int rc = from->get_current(&key, &data);
    while (rc == MDB_SUCCESS) {
        // Extraction occurs here
        silkworm::ByteView k{silkworm::db::from_mdb_val(key)};
        silkworm::ByteView v{silkworm::db::from_mdb_val(data)};
        // find n and total length
        uint32_t n = boost::endian::load_big_u32(v.substr(0, 4).data());

        if (n == 0) {
            rc = from->get_next(&key, &data);
            continue;
        }
        uint32_t offset = 4 + n*HASH_LENGTH + 4*n;

        for (uint32_t i = 0; i < n; i++)
        {
            silkworm::ByteView changeset_key{v.substr(4+i*HASH_LENGTH, 4+(i+1)*HASH_LENGTH)};
            uint32_t idx_0 = 0;
            if (i > 0) {
                idx_0 = boost::endian::load_big_u32(v.substr(4+n*HASH_LENGTH+4*(i-1), 4+n*HASH_LENGTH+4*i).data());
            }
            uint32_t idx_1 = boost::endian::load_big_u32(v.substr(4+n*HASH_LENGTH+4*i, 4+n*HASH_LENGTH+4*(i+1)).data());
            silkworm::ByteView changeset_value{v.substr(offset+idx_0, offset+idx_1)};
            // Collection
            uint64_t block_number = decode_number(silkworm::Bytes(k.data()));
            silkworm::Bytes value(9, '\0');
            boost::endian::store_big_u64(&value[0], block_number);
            if (changeset_value.size() == 0) value[8] = 1;
            else value[8] = 0;
            collector.collect(changeset_key, silkworm::ByteView(value.data()));
        }
        // END of extraction
        rc = from->get_next(&key, &data);
        if (rc && rc != MDB_NOTFOUND) {
            return rc;
        }
    }
    collector.load(to.get());
    return 0;
}
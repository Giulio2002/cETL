/*
   Copyright 2020 The Silkworm Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef CETL_TG_API_H_
#define CETL_TG_API_H_


#include <lmdb/lmdb.h>
#include <stdbool.h>
#include <stdint.h>

#define OPTIMAL_BUFFER_SIZE 268435456
#define HASH_LENGTH 32

#if defined _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#if __cplusplus
extern "C" {
#endif

// EXPORT int tg_index_accounts(MDB_txn* txn, uint64_t start_block);
EXPORT int tg_blockhashes(MDB_txn* mdb_txn, uint64_t block_number);

#if __cplusplus
}
#endif

#endif

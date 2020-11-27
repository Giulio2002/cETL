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

#include "heap.hpp"

#include <catch2/catch.hpp>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

using namespace etl;

namespace silkworm {

namespace {
    bool test_heap() {
        Heap h = new_heap();
        push_heap(&h, {silkworm::ByteView({1}), silkworm::ByteView({34}), 0});
        push_heap(&h, {silkworm::ByteView({8}), silkworm::ByteView({34}), 0});
        push_heap(&h, {silkworm::ByteView({6}), silkworm::ByteView({34}), 0});
        push_heap(&h, {silkworm::ByteView({2}), silkworm::ByteView({34}), 0});

        if (pop_heap(&h).key.at(0) != 0x1) return false;
        if (pop_heap(&h).key.at(0) != 0x2) return false;
        if (pop_heap(&h).key.at(0) != 0x6) return false;
        if (pop_heap(&h).key.at(0) != 0x8) return false;
        return true;
    }
}  // namespace

TEST_CASE("Heap") {
    CHECK(test_heap());
}

}  // namespace silkworm

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

#include "appearBuffer.hpp"

#include <catch2/catch.hpp>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace silkworm {

namespace {


    size_t optimal = 30;

    bool test_appearBuffer() {
        Buffer *b = new AppearBuffer(optimal);
        b->put(silkworm::ByteView({4, 1, 5, 6, 5}), silkworm::ByteView({0x45, 0x61, 0x9, 0x6, 0x1}));
        b->put(silkworm::ByteView({9, 1, 5, 6, 5}), silkworm::ByteView({0x15, 0x21, 0x9, 0x6, 0x1}));
        b->put(silkworm::ByteView({1, 1, 5, 6, 5}), silkworm::ByteView({0x10, 0x21, 0x9, 0x6, 0x1}));
        if (!b->checkFlushSize()) return false;
        b->sort();
        auto e = b->getEntries();
        if (e.at(0).k.at(0) != 0x1) return false;
        if (e.at(1).k.at(0) != 0x4) return false;
        if (e.at(2).k.at(0) != 0x9) return false;
        return true;
    }
}  // namespace

TEST_CASE("AppearBuffer") {
    CHECK(test_appearBuffer());
}

}  // namespace silkworm

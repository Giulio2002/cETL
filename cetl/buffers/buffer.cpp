#include <cetl/buffers/buffer.hpp>
#include <algorithm>

Buffer::Buffer(size_t _optimalSize) {
    optimalSize = _optimalSize;
    entries = std::vector<entry>();
    size = 0;
}

void Buffer::put(silkworm::ByteView k, silkworm::ByteView v) {
    size += v.length() + k.length();
    entries.push_back({k, v});
}

void Buffer::sort() {
    std::sort(entries.begin(), entries.end(), [](const entry lhs, const entry rhs) {
        return lhs.k.compare(rhs.k) >= 0;
    });
}


std::vector<entry>::iterator Buffer::begin() {
    return entries.begin();
}

std::vector<entry>::iterator Buffer::end() {
    return entries.end();
}

int Buffer::length() {
    return entries.size();
}

void Buffer::reset() {
    entries.clear();
    entries.shrink_to_fit();
    size = 0;
}

bool Buffer::checkFlushSize() {
    return entries.size() >= optimalSize;
}
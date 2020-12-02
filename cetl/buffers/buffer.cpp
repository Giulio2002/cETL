#include <cetl/buffers/buffer.hpp>
#include <algorithm>

Buffer::Buffer(size_t _optimalSize) {
    optimalSize = _optimalSize;
    entries = std::map<silkworm::ByteView, silkworm::ByteView>();
    size = 0;
}

void Buffer::put(silkworm::ByteView k, silkworm::ByteView v) {
    size += v.length() + k.length();
    entries.insert({k, v});
}

std::map<silkworm::ByteView, silkworm::ByteView>::iterator Buffer::begin() {
    return entries.begin();
}

std::map<silkworm::ByteView, silkworm::ByteView>::iterator Buffer::end() {
    return entries.end();
}

int Buffer::length() {
    return entries.size();
}

void Buffer::reset() {
    entries.clear();
    size = 0;
}

bool Buffer::checkFlushSize() {
    return entries.size() >= optimalSize;
}
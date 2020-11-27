#include <cetl/buffers/appendBuffer.hpp>

AppendBuffer::AppendBuffer(size_t _optimalSize) {
    optimalSize = _optimalSize;
    entries = std::map<silkworm::ByteView, silkworm::ByteView>();
    buffer = std::vector<entry>();
    size = 0;
}

void AppendBuffer::put(silkworm::ByteView k, silkworm::ByteView v) {
    auto e = entries.find(k);
    silkworm::ByteView stored;
	if (entries.end() == e) {
        size += k.length();
    } else {
        stored = e->second;
    }
    size += v.length();
    char * _stored = (char *) malloc(sizeof(char) * (stored.size() + v.size()));
    strcat(_stored, (const char *) stored.data());
    strcat(_stored, (const char *) v.data());
    stored = silkworm::ByteView((const unsigned char *) _stored, sizeof _stored);
    entries.insert({k, stored});
}

entry AppendBuffer::get(int i) {
    return buffer.at(i);
}

std::vector<entry> AppendBuffer::getEntries() {
    return buffer;
}

int AppendBuffer::length() {
    return entries.size();
}

void AppendBuffer::sort() {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        buffer.push_back({it->first, it->second});
    }
    std::sort(buffer.begin(), buffer.end(), [](const entry lhs, const entry rhs) {
        return lhs.k.compare(rhs.k) < 0;
    });
}

void AppendBuffer::reset() {
    entries.clear();
    buffer.clear();
    size = 0;
}

bool AppendBuffer::checkFlushSize() {
    return size >= optimalSize;
}
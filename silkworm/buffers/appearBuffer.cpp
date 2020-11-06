#include <silkworm/buffers/appearBuffer.hpp>
#include <algorithm>

AppearBuffer::AppearBuffer(size_t _optimalSize) {
    optimalSize = _optimalSize;
    entries = std::map<std::string, std::string>();
    buffer = std::vector<entry>();
    size = 0;
}

void AppearBuffer::put(std::string k, std::string v) {
    auto e = entries.find(k);
	if (entries.end() != e) {
        return;
    }
    size += v.length() + k.length();
    entries.insert({k, v});
}

entry AppearBuffer::get(int i) {
    return buffer.at(i);
}

std::vector<entry> AppearBuffer::getEntries() {
    return buffer;
}

int AppearBuffer::length() {
    return entries.size();
}

void AppearBuffer::sort() {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        buffer.push_back({it->first, it->second});
    }
    std::sort(buffer.begin(), buffer.end(), [](const entry lhs, const entry rhs) {
        return lhs.k.compare(rhs.k) < 0;
    });
}

void AppearBuffer::reset() {
    entries.clear();
    buffer.clear();
    size = 0;
}

bool AppearBuffer::checkFlushSize() {
    return size >= optimalSize;
}
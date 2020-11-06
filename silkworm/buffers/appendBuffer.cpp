#include <silkworm/buffers/appendBuffer.hpp>

AppendBuffer::AppendBuffer(size_t _optimalSize) {
    optimalSize = _optimalSize;
    entries = std::map<std::string, std::string>();
    buffer = std::vector<entry>();
    size = 0;
}

void AppendBuffer::put(std::string k, std::string v) {
    auto e = entries.find(k);
    std::string stored;
	if (entries.end() == e) {
        size += k.length();
    } else {
        stored = e->second;
    }
    size += v.length();
    stored = stored + v;
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
    std::sort(buffer.begin(), buffer.end(), comp);
}

void AppendBuffer::reset() {
    entries.clear();
    buffer.clear();
    size = 0;
}

bool AppendBuffer::checkFlushSize() {
    return size >= optimalSize;
}
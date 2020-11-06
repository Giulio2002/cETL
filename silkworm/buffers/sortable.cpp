#include <silkworm/buffers/sortable.hpp>

SortableBuffer::SortableBuffer(size_t _optimalSize) {
    optimalSize = _optimalSize;
    entries = std::vector<entry>();
    size = 0;
}

void SortableBuffer::put(std::string k, std::string v) {
    size += k.length() + v.length();
    entries.push_back(entry{k, v});
}

entry SortableBuffer::get(int i) {
    return entries.at(i);
}

std::vector<entry> SortableBuffer::getEntries() {
    return entries;
}

int SortableBuffer::length() {
    return entries.size();
}

void SortableBuffer::sort() {
    std::sort(entries.begin(), entries.end(), comp);
}

void SortableBuffer::reset() {
    entries.clear();
    size = 0;
}

bool SortableBuffer::checkFlushSize() {
    return size >= optimalSize;
}
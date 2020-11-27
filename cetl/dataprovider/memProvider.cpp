#include <cetl/dataprovider/memProvider.hpp>

memProvider::memProvider(Buffer * _buffer) {
    buffer = _buffer;
    pos = 0;
}

entry memProvider::next() {
    if (pos >= buffer->length())
        return {std::string(), std::string()};

    entry e = buffer->get(pos);
    pos++;
    return e;
}

void memProvider::reset() {
    return;
}
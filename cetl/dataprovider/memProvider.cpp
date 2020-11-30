#include <cetl/dataprovider/memProvider.hpp>

memProvider::memProvider(SortableBuffer * _buffer) {
    buffer = _buffer;
    pos = 0;
}

entry memProvider::next() {
    if (pos >= buffer->length()) {
        buffer->reset();
        return {silkworm::ByteView(), silkworm::ByteView()};
    }

    entry e = buffer->get(pos);
    pos++;
    return e;
}

void memProvider::reset() {
    return;
}
#include <silkworm/heap/heap.hpp>

namespace etl {

Heap new_heap() {
    auto h = Heap();
    std::make_heap(h.begin(), h.end(), [](const heap_elem lhs, const heap_elem rhs) {
        return lhs.key.compare(rhs.key) >= 0;
    });
    return h;
}

heap_elem pop_heap(Heap *h) {
    auto front = h->front();
    std::pop_heap (h->begin(), h->end(), [](const heap_elem lhs, const heap_elem rhs) {
        return lhs.key.compare(rhs.key) >= 0;
    });
    h->pop_back();
    return front;
}

void push_heap(Heap *h, heap_elem e) {
    h->push_back(e);
    std::push_heap(h->begin(), h->end(), [](const heap_elem lhs, const heap_elem rhs) {
        return lhs.key.compare(rhs.key) >= 0;
    });
}

}
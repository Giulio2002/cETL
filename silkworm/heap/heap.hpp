#include <vector>
#include <algorithm>
#include <string>

namespace etl{

struct heap_elem {
    std::string key;
    std::string value;
    int time;
};

typedef std::vector<heap_elem> Heap;

Heap new_heap();
heap_elem pop_heap(Heap *);
void push_heap(Heap *, heap_elem);

}
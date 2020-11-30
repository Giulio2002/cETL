#include <cetl/collector/collector.hpp>
#include <cetl/dataprovider/fileProvider.hpp>
#include <cetl/dataprovider/memProvider.hpp>
#include <cetl/heap/heap.hpp>

Collector::Collector(SortableBuffer * _b) {
    b = _b;
    dataProviders = std::vector<DataProvider *>();
}

void Collector::flushBuffer(bool ram) {
    b->sort();
    if (b->length() == 0) {
        return;
    }
    if (ram && dataProviders.size() == 0) {
        dataProviders.push_back(new memProvider(b));
    } else {
        dataProviders.push_back(new fileProvider(b, dataProviders.size()));
        b->reset();
    }
}

void Collector::collect(silkworm::ByteView k, silkworm::ByteView v) {
    b->put(k, v);
    if (b->checkFlushSize()) {
        flushBuffer(false);
    }
}

void Collector::load(silkworm::lmdb::Table * t, OnLoad load) {
    etl::Heap h = etl::new_heap();
    flushBuffer(false);
    for (unsigned int i = 0; i < dataProviders.size(); i++)
    {
        auto entry = dataProviders.at(i)->next();
        etl::push_heap(&h, {entry.k, entry.v, (int)i});
    }
    size_t s = 0;
    while (h.size() > 0) {
		auto e = etl::pop_heap(&h);
        load(e.key, e.value);
        t->put(e.key, e.value);
        s++;
		auto next = dataProviders.at(e.time)->next();
        if (next.k.size() ==  0 && next.v.size() ==  0) {
            dataProviders.at(e.time)->reset();
            dataProviders.erase(dataProviders.begin() + e.time);
            continue;
        }
        etl::push_heap(&h, {next.k, next.v, e.time});
    }
}

void Collector::load(silkworm::lmdb::Table * t) {
    etl::Heap h = etl::new_heap();
    flushBuffer(true);

    for (unsigned int i = 0; i < dataProviders.size(); i++)
    {
        auto entry = dataProviders.at(i)->next();
        etl::push_heap(&h, {entry.k, entry.v, (int)i});
    }
    size_t s = 0;
    while (h.size() > 0) {
		auto e = etl::pop_heap(&h);
        auto key = silkworm::db::to_mdb_val(e.key);
        auto value = silkworm::db::to_mdb_val(e.value);
        t->put_append(&key, &value);
        s++;
		auto next = dataProviders.at(e.time)->next();
        if (next.k.size() ==  0 && next.v.size() ==  0) {
            dataProviders.at(e.time)->reset();
            dataProviders.erase(dataProviders.begin() + e.time);
            dataProviders.shrink_to_fit();
            continue;
        }
        etl::push_heap(&h, {next.k, next.v, e.time});
    }
}
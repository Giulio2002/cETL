#include <cetl/collector/collector.hpp>
#include <cetl/dataprovider/fileProvider.hpp>
#include <cetl/heap/heap.hpp>

Collector::Collector(SortableBuffer * _b) {
    b = _b;
    dataProviders = std::vector<FileProvider *>();
}

void Collector::flushBuffer() {
    b->sort();
    if (b->length() == 0) {
        return;
    }

    dataProviders.push_back(new FileProvider(b, dataProviders.size()));
    b->reset();

}

void Collector::collect(silkworm::ByteView k, silkworm::ByteView v) {
    b->put(k, v);
    if (b->checkFlushSize()) {
        flushBuffer();
    }
}

void Collector::load(silkworm::lmdb::Table * t, OnLoad load) {
    if (dataProviders.size() == 0) {
        b->sort();
        auto entries = b->getEntries();
        for (auto e: entries) {
            auto key = silkworm::db::to_mdb_val(e.k);
            auto value = silkworm::db::to_mdb_val(e.v);
            t->put_append(&key, &value);
        }
        return;
    }

    etl::Heap h = etl::new_heap();


    flushBuffer();
    for (unsigned int i = 0; i < dataProviders.size(); i++)
    {
        auto entry = dataProviders.at(i)->next();
        etl::push_heap(&h, {entry.k, entry.v, (int)i});
    }

    while (h.size() > 0) {
		auto e = etl::pop_heap(&h);
        load(e.key, e.value);
        auto key = silkworm::db::to_mdb_val(e.key);
        auto value = silkworm::db::to_mdb_val(e.value);
        t->put_append(&key, &value);
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
    if (dataProviders.size() == 0) {
        b->sort();
        auto entries = b->getEntries();
        for (auto e: entries) {
            t->put(e.k, e.v);
        }
        return;
    }

    etl::Heap h = etl::new_heap();
    flushBuffer();

    for (unsigned int i = 0; i < dataProviders.size(); i++)
    {
        auto entry = dataProviders.at(i)->next();
        etl::push_heap(&h, {entry.k, entry.v, (int)i});
    }
    size_t s = 0;
    while (h.size() > 0) {
		auto e = etl::pop_heap(&h);
        t->put(e.key, e.value);
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
#include <cetl/collector/collector.hpp>
#include <cetl/dataprovider/fileProvider.hpp>
#include <cetl/dataprovider/memProvider.hpp>
#include <cetl/heap/heap.hpp>

Collector::Collector(std::string _dir, bool _autoclean, SortableBuffer * _b) {
    dir = _dir;
    b = _b;
    autoclean = _autoclean;
    dataProviders = std::vector<DataProvider *>();
}

Collector::Collector(std::string _dir) {
   autoclean = false;
   dataProviders = std::vector<DataProvider *>();
   struct dirent *entry;
   dir = _dir;

   DIR *d = opendir(dir.c_str());

   if (d == NULL) {
      return;
   }
   while ((entry = readdir(d)) != NULL) {
       dataProviders.push_back(new fileProvider(std::string(entry->d_name)));
   }
   closedir(d);
}

void Collector::flushBuffer(bool ram) {
    b->sort();
    if (b->length() == 0) {
        return;
    }
    if (ram && dataProviders.size() == 0) {
        dataProviders.push_back(new memProvider(b));
        allFlushed = true;
    } else {
        dataProviders.push_back(new fileProvider(b, std::string("cetl")));
    }
}

void Collector::collect(silkworm::ByteView k, silkworm::ByteView v) {
    b->put(k, v);
    if (b->checkFlushSize()) {
        flushBuffer(false);
    }
}

void Collector::load(std::unique_ptr<silkworm::lmdb::Table> t, silkworm::lmdb::Transaction * tx, OnLoad load) {
    etl::Heap h = etl::new_heap();

    for (unsigned int i = 0; i < dataProviders.size(); i++)
    {
        auto entry = dataProviders.at(i)->next();
        etl::push_heap(&h, {entry.k, entry.v, (int)i});
    }
    size_t s = 0;
    while (h.size() > 0) {
		auto e = etl::pop_heap(&h);
        load(e.key, e.value);
        // t->put(e.key, e.value);
        s++;
		auto next = dataProviders.at(e.time)->next();
        tx->commit();
        t->clear();
        if (next.k.size() ==  0 && next.v.size() ==  0) {
            dataProviders.at(e.time)->reset();
            dataProviders.erase(dataProviders.begin() + e.time);
            continue;
        }
        etl::push_heap(&h, {next.k, next.v, e.time});
        if (s%100000 == 0)
            tx->commit();
    }
}

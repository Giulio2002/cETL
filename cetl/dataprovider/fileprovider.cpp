#include <cetl/dataprovider/fileProvider.hpp>
#include <boost/endian/conversion.hpp>
#include <string.h>
#include <cstdio>
#include <stdlib.h>
#include <sys/stat.h>

silkworm::ByteView from_char_array(void * val, size_t size) {
    auto* ptr{static_cast<uint8_t*>(val)};
    return {ptr, size};
}

std::string byteviewToString(silkworm::ByteView bytes) {
    auto res = std::string();
    for (unsigned int i = 0; i < bytes.size(); i++) {
        res.push_back((char) bytes.at(i));
    }
    return res;

}
FileProvider::FileProvider(SortableBuffer * b, int i) {
    file.open("./tmp"+i, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
    auto entries = b->getEntries();
    for (auto e: entries) {
        /*silkworm::Bytes kLen(4, '\0');
        silkworm::Bytes vLen(4, '\0');
        boost::endian::store_big_u32(&kLen[0], e.k.size());
        boost::endian::store_big_u32(&vLen[0], e.v.size());*/
        auto flow = std::string();
        flow.push_back((char) e.k.size());
        flow.push_back((char) e.v.size());
        flow.append(byteviewToString(e.k));
        flow.append(byteviewToString(e.v));
        file.write(flow.c_str(), flow.size());
    }
    file.seekp(0);
}

FileProvider::FileProvider(std::string _filename) {
    filename = (char *) _filename.c_str();
    std::fstream s = std::fstream(filename);
    file.seekp(0);
}

entry FileProvider::next() {
    if (file.eof()) {
        return {silkworm::ByteView(), silkworm::ByteView()};
    }
    char * s_buffer = (char*) malloc(sizeof(char) * 2);
    file.read(s_buffer, 2);
    auto u_buffer = (unsigned char *) s_buffer;
    auto kLength = (unsigned int) u_buffer[0];
    auto vLength = (unsigned int) u_buffer[1];
    char * s_kv = (char *) malloc(sizeof(char) * (kLength+ vLength));
    unsigned char k[kLength];
    unsigned char v[vLength];

    file.read(s_kv, kLength+vLength);
    auto u_kv = (unsigned char *) s_kv;
    for (unsigned int i = 0; i < kLength; i++)
    {
        k[i] = u_kv[i];
    }

    for (unsigned int i = kLength; i < kLength+vLength; i++)
    {
        v[i] = u_kv[i];
    }

    free(s_buffer);
    free(s_kv);

    return {from_char_array(k, kLength), from_char_array(v, vLength)};
}

void FileProvider::reset() {
    std::remove(filename);
}
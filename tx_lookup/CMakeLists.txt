find_package(Microsoft.GSL CONFIG REQUIRED)

add_library(cetl_lookup_api SHARED api.h api.cpp)
target_link_libraries(cetl_lookup_api PUBLIC lmdb PRIVATE silkworm cetl Microsoft.GSL::GSL)
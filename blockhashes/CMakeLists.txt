find_package(Microsoft.GSL CONFIG REQUIRED)

add_library(cetl_blockhashes_api SHARED api.h api.cpp)
target_link_libraries(cetl_blockhashes_api PUBLIC lmdb PRIVATE silkworm cetl Microsoft.GSL::GSL)
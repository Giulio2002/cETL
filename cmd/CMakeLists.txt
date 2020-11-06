find_package(absl CONFIG REQUIRED)
#find_package(Boost CONFIG REQUIRED COMPONENTS filesystem)

if(MSVC)
  add_link_options(/STACK:10000000)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_link_options(-Wl,-stack_size -Wl,0x1000000)
endif()

find_package(CLI11 CONFIG REQUIRED)

# Unit tests
enable_testing()

find_package(Catch2 CONFIG REQUIRED)

file(GLOB_RECURSE SILKWORM_TESTS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/silkworm/*_test.cpp")
add_executable(unit_test unit_test.cpp ${SILKWORM_TESTS})
target_link_libraries(unit_test silkworm Catch2::Catch2)
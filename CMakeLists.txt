#[[
   Copyright 2020 The Silkworm Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
]]

cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_INIT "-fPIC" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_INIT "-fPIC" CACHE STRING "" FORCE)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/silkworm/cmake/toolchain.cmake CACHE FILEPATH "" FORCE)

include(silkworm/evmone/cmake/cable/HunterGate.cmake)

include(silkworm/evmone/cmake/cable/bootstrap.cmake)
include(CableBuildType)
cable_set_build_type(DEFAULT Release CONFIGURATION_TYPES Release Debug)

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.267.tar.gz"
  SHA1 "9c5c7fa6e17c2ae15dd922184bc7c51235aaae70"
  FILEPATH "${CMAKE_SOURCE_DIR}/silkworm/cmake/Hunter/config.cmake"
)

project(cetl)

# Profing and coverage options
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang$")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # coverage
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping)
  else()
    # profiling
    add_compile_options(-gline-tables-only)
  endif()
endif()

find_package(absl CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS filesystem)
find_package(cryptopp CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(ethash CONFIG REQUIRED)
find_package(intx CONFIG REQUIRED)

# silkworm
add_subdirectory(silkworm)

# Warnings options
if(MSVC)
  add_compile_definitions(_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)

  add_compile_options(/wd5030) # Silence warnings about GNU attributes
  add_compile_options(/wd4324) # Silence warning C4324: 'xxx': structure was padded due to alignment specifier
  add_compile_options(/wd4068) # Silence warning C4068: unknown pragma
  add_compile_options(/W4)

  add_link_options(/ignore:4099)
else()
  add_compile_options(-Wall -Wextra -Werror -Wno-missing-field-initializers)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wno-attributes)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_compile_definitions(_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS)
  add_compile_options(-Wthread-safety)
endif()



# CETL itself
add_subdirectory(cetl)
add_subdirectory(api)


cmake_minimum_required(VERSION 3.20)
project(io-uring-soup-bin LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON) # verbose CMake output

# standard CMake build types: Debug, Release, RelWithDebInfo, MinSizeRel
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

set(CMAKE_LINKER_TYPE LLD)
set(CMAKE_LINKER "$ENV{LLVM_HOME}/bin/lld")
message("CMAKE_LINKER=${CMAKE_LINKER}")

set(CMAKE_CXX_STANDARD $ENV{CPP_STANDARD})
set(CMAKE_CXX_EXTENSIONS OFF) # Disable GNU extensions
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate a compile_commands.json file, needed for clang-tidy

# https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#gcc--clang
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wsign-conversion -Werror")
set(MY_CXX_FLAGS -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wsign-conversion -Werror)

# set in conan profile
#set(CMAKE_CXX_FLAGS_DEBUG "-gdwarf-4 -O0") # valgrind needs `-gdwarf-4`
#set(CMAKE_CXX_FLAGS_DEBUG "-g -gdwarf-4 -O3 -DNDEBUG") # `-03 -DNDEBUG` for gperftools

#
# tools
#

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message("Found ccache: ${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

#
# dependencies
#

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS log)

# example uses this to compute hash digest of sent and received messages
find_package(xxHash REQUIRED)

# example uses this to measure and report message latency
find_package(hdr_histogram REQUIRED)

find_package(GTest REQUIRED)

find_package(rapidcheck REQUIRED)

# system include directories do not raise compiler warnings
include_directories(
  SYSTEM ${Boost_INCLUDE_DIR}
  SYSTEM ${xxHash_INCLUDE_DIR}
  SYSTEM ${hdr_histogram_INCLUDE_DIR}
  # SYSTEM ${rapidcheck_INCLUDE_DIR}
)

#
# artifacts
#

add_library(${PROJECT_NAME}
  STATIC
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE util
)

add_library(util
  OBJECT src/util/boost_log.cpp
)
target_compile_options(util
  PRIVATE ${MY_CXX_FLAGS}
)

enable_testing()
include(GoogleTest)

# add_executable(${xyz}
#   src/test/${xyz}.cpp
# )
# target_link_libraries(${xyz}
#   PRIVATE sequencer
#   PRIVATE shm_util
#   PRIVATE logging
#   PRIVATE actor_id
#   PRIVATE demux-cpp::demux-cpp
#   PRIVATE gtest::gtest
#   PRIVATE rapidcheck
# )
# target_compile_options(${xyz}
#   PRIVATE ${MY_CXX_FLAGS}
# )
# gtest_discover_tests(${xyz})

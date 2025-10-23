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
# Conan dependencies
#

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS log)

# example uses this to compute hash digest of sent and received messages
find_package(xxHash REQUIRED)

# example uses this to measure and report message latency
find_package(hdr_histogram REQUIRED)

find_package(GTest REQUIRED)

find_package(rapidcheck REQUIRED)

#
# Linux-specific dependencies
#

find_package(PkgConfig REQUIRED)

pkg_check_modules(liburing REQUIRED IMPORTED_TARGET GLOBAL liburing)

if (liburing_FOUND)
    message(STATUS "Found liburing version ${liburing_VERSION}")
endif()

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

add_executable(server_example
  src/example/server_example.cpp
)
target_link_libraries(server_example
  PRIVATE server
  PRIVATE util
  PRIVATE hdr_histogram::hdr_histogram_static
  PRIVATE dl # boost/stacktrace.hpp
  PRIVATE Boost::log
)
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_link_libraries(server_example PRIVATE profiler)
endif()
target_compile_options(server_example
  PRIVATE ${MY_CXX_FLAGS}
)

add_executable(client_example
  src/example/client_example.cpp
)
target_link_libraries(client_example
  PRIVATE client
  PRIVATE util
  PRIVATE hdr_histogram::hdr_histogram_static
  PRIVATE dl # boost/stacktrace.hpp
  PRIVATE Boost::log
)
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_link_libraries(client_example PRIVATE profiler)
endif()
target_compile_options(client_example
  PRIVATE ${MY_CXX_FLAGS}
)

add_library(${PROJECT_NAME}
  STATIC
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE server
  PRIVATE client
  PRIVATE util
  PRIVATE PkgConfig::liburing
)

add_library(server
  OBJECT src/core/server.cpp
)
target_compile_options(server
  PRIVATE ${MY_CXX_FLAGS}
)

add_library(client
  OBJECT src/core/client.cpp
)
target_compile_options(client
  PRIVATE ${MY_CXX_FLAGS}
)

add_library(util
  OBJECT src/util/boost_log.cpp
)
target_compile_options(util
  PRIVATE ${MY_CXX_FLAGS}
)

enable_testing()
include(GoogleTest)

add_executable(server_test
  src/test/server_test.cpp
)
target_link_libraries(server_test
  PRIVATE server
  PRIVATE util
  PRIVATE gtest::gtest
  PRIVATE rapidcheck
  PRIVATE Boost::log
)
target_compile_options(server_test
  PRIVATE ${MY_CXX_FLAGS}
)
gtest_discover_tests(server_test)

add_executable(client_test
  src/test/client_test.cpp
)
target_link_libraries(client_test
  PRIVATE client
  PRIVATE util
  PRIVATE gtest::gtest
  PRIVATE rapidcheck
  PRIVATE Boost::log
)
target_compile_options(client_test
  PRIVATE ${MY_CXX_FLAGS}
)
gtest_discover_tests(client_test)

# add_executable(${xyz}
#   src/test/${xyz}.cpp
# )
# target_link_libraries(${xyz}
#   PRIVATE server
#   PRIVATE client
#   PRIVATE util
#   PRIVATE gtest::gtest
#   PRIVATE rapidcheck
#   PRIVATE Boost::log
# )
# target_compile_options(${xyz}
#   PRIVATE ${MY_CXX_FLAGS}
# )
# gtest_discover_tests(${xyz})

cmake_minimum_required(VERSION 3.1)
project(nash)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules")
set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/3rdParty)

find_package(GMP REQUIRED)
message(STATUS "GMP library found.")
message(STATUS "GMP include dir is: ${GMP_INCLUDE_DIR}")
message(STATUS "GMP library is: ${GMP_LIBRARY}")

link_directories(${GMP_LIBRARY})
include_directories(${GMP_INCLUDE_DIR})

add_executable(nash "src/nash.cpp" "src/NashSieve.cpp" "src/arg_parser.cc")
add_executable(mnash "src/mnash.cpp" "src/NashSieve.cpp" "src/arg_parser.cc")
target_link_libraries(nash ${GMP_LIBRARY})
target_link_libraries(mnash ${GMP_LIBRARY})

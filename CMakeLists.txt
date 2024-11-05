cmake_minimum_required(VERSION 3.11)
project(SearchTree)
set(CMAKE_CXX_FLAGS "-O2 -std=c++20")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0 -fsanitize=address -fsanitize=undefined")

set(SRCS
	${CMAKE_SOURCE_DIR}/SearchTree/src/search_tree.cpp
)
add_executable(SearchTree.x)

target_sources(SearchTree.x PRIVATE ${SRCS})
target_include_directories(SearchTree.x PUBLIC "${CMAKE_SOURCE_DIR}/SearchTree/include")

add_subdirectory(${CMAKE_SOURCE_DIR}/tests)


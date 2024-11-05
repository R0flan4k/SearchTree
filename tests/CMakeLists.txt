cmake_minimum_required(VERSION 3.11)

project(SearchTreeTests)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

include_directories(
	${CMAKE_SOURCE_DIR}/SearchTree/include
	${GTEST_INCLUDE_DIRS}
)

add_executable(
	TestSerialization ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)

target_link_libraries(
	TestSerialization PUBLIC ${GTEST_LIBRARIES} gtest::gtest)

enable_testing()
add_test(TestSerialization "./TestSerialization")

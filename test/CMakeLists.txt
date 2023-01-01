cmake_minimum_required(VERSION 2.8.9)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(test_client ../client/src/test.cpp ../client/src/io.cpp)
add_executable(test_server ../server/src/test.cpp ../server/src/server.cpp)
target_link_libraries(test_client ${GTEST_LIBRARIES} pthread)
target_link_libraries(test_server ${GTEST_LIBRARIES} pthread mysqlcppconn)

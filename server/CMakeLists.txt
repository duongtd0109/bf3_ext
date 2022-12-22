cmake_minimum_required(VERSION 2.8.9)
project(Server)

set(SOURCES src/main.cpp src/server.cpp)
set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR /usr/local/include /usr/local/include/cppconn)

include_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include)
include_directories(include)
include_directories(external)

link_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/lib)

add_executable(Server ${SOURCES})
target_link_libraries(Server mysqlcppconn)
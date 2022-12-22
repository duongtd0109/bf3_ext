cmake_minimum_required(VERSION 2.8.9)

project(Client)

set(SRC ./src/main.cpp ./src/client.cpp ./src/io.cpp ./src/sha256.cpp)

include_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include)
include_directories(include)
include_directories(external)

add_executable(Client ${SRC})

target_link_libraries(Client pthread)

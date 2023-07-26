cmake_minimum_required(VERSION 3.5)

project(SIMPLE_HTTP_SERVER)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*" "src/utility/*")

add_executable(simple_http_server ${SOURCES})

# Optionally, you can include additional source files like this:
# add_executable(simple_http_server main.cpp additional_file.cpp)

target_include_directories(simple_http_server PRIVATE include)

# target_link_libraries(simple_http_server PRIVATE library_name)

target_compile_options(simple_http_server PRIVATE -Wall -Wextra)
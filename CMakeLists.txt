cmake_minimum_required(VERSION 3.10)
project(klv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(klv STATIC
    core/klv.cpp
    core/stanag.cpp
    st0102/st0102.cpp
    st0601/st0601.cpp
    st0903/st0903.cpp
)

target_include_directories(klv PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/st0102
    ${CMAKE_CURRENT_SOURCE_DIR}/st0601
    ${CMAKE_CURRENT_SOURCE_DIR}/st0903
)

add_executable(example
    example/main.cpp
)

target_link_libraries(example PRIVATE klv)

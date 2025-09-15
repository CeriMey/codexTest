cmake_minimum_required(VERSION 3.10)
project(klv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_library(klv STATIC
    core/klv_leaf.cpp
    core/klv_bytes.cpp
    core/klv_set.cpp
    core/klv_registry.cpp
    core/stanag.cpp
    st0102/st0102.cpp
    st0601/st0601.cpp
    st0903/st0903.cpp
    core/klv.h
    core/klv_types.h
    core/klv_node.h
    core/klv_leaf.h
    core/klv_bytes.h
    core/klv_set.h
    core/klv_registry.h
    core/stanag.h
    st0102/st0102.h
    st0601/st0601.h
    st0903/st0903.h
    core/klv_macros.h
    core/st_common.h
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

add_executable(macro_example
    example/macro_example.cpp
)

target_link_libraries(macro_example PRIVATE klv)

add_executable(stanag4609_simple
    example/stanag4609_simple.cpp
)

target_link_libraries(stanag4609_simple PRIVATE klv)

add_executable(klv_tests
    tests/encode_tests.cpp
)

target_link_libraries(klv_tests PRIVATE klv)

add_test(NAME klv_encode_tests COMMAND klv_tests)

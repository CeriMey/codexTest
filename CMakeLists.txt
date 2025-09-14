cmake_minimum_required(VERSION 3.10)
project(klv_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main
    main.cpp
    klv.cpp
    st0102.cpp
    st0601.cpp
    st0903.cpp
    stanag.cpp
)

target_compile_options(main PRIVATE -Wall -Wextra -pedantic -O2)

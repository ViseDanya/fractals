cmake_minimum_required(VERSION 3.30)
project(Fractals C)
set(CMAKE_CXX_STANDARD 11)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCE_FILES
    src/main.c
    src/shader.c
    src/shader.h
)

add_executable(main ${SOURCE_FILES})
target_link_libraries(main PRIVATE SDL3::SDL3 OpenGL::GL)


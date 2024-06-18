
cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_COMPILER /usr/bin/clang)
project(Jaoned LANGUAGES CXX C)

add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Wpedantic")
add_compile_options("-O3")
#add_link_options("-s")

add_compile_options("-Wno-c99-extensions")
add_compile_options("-Wno-c11-extensions")

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options("-Wno-nullability-extension")
endif()

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image SDL2_ttf GL GLEW ${PROJECT_SOURCE_DIR}/libs/dnealar/bin/libDnealar.so)
target_include_directories(${PROJECT_NAME} PRIVATE include ${PROJECT_SOURCE_DIR}/libs/dnealar/include)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})


cmake_minimum_required(VERSION 3.26)
project(Jaoned LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Wpedantic")
add_compile_options("-O3")
#add_link_options("-s")

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt::Core Qt::Gui Qt::Widgets Qt::OpenGL Qt::OpenGLWidgets)

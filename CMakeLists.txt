cmake_minimum_required(VERSION 3.10)
project(paintclone)

find_package(GTK3 REQUIRED)

CMAKE_INCLUDE_PATH(/opt/homebrew/Cellar/gtk+3/3.24.43/include/gtk-3.0)

add_executable(paintclone paintclone.cpp)

target_link_libraries(paintclone PRIVATE gtk+3.0)
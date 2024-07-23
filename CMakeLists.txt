cmake_minimum_required(VERSION 3.10)
project(paintclone)

find_package(GTK3 REQUIRED HINTS /opt/homebrew/Cellar/gtk+3/3.24.43/include)

add_executable(paintclone paintclone.cpp)

target_link_libraries(paintclone PRIVATE gtk+3.0)
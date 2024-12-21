cmake_minimum_required(VERSION 3.5.0)
project(C-Raycaster VERSION 0.1.0 LANGUAGES C)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(C-Raycaster src/main.c)
target_link_libraries(C-Raycaster ${SDL2_LIBRARIES})


cmake_minimum_required(VERSION 3.5.0)
project(C-Raycaster VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/releases/download/${RAYLIB_VERSION}/raylib-${RAYLIB_VERSION}_linux_amd64.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(raylib)
FetchContent_Declare(
    fmt 
    URL https://github.com/fmtlib/fmt/releases/download/11.1.4/fmt-11.1.4.zip
    FIND_PACKAGE_ARGS 11.1.4
)
FetchContent_MakeAvailable(fmt)


set(SOURCE_FILES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} fmt)

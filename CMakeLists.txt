cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(game_engine LANGUAGES CXX)


if(WIN32)

endif()

if(MSVC)

endif()

add_subdirectory(engine)
add_subdirectory(sanbox)

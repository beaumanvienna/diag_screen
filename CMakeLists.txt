cmake_minimum_required(VERSION 3.10)

project(diag_screen)

add_subdirectory(ppsspp/Common)
add_subdirectory(ppsspp/UI)
add_subdirectory(ppsspp/SDL)

add_executable(DIAG_SCREEN tests/diag_screen/main.cpp)

target_link_libraries(DIAG_SCREEN PUBLIC common ui sdl common ui sdl common ui sdl)

set (CMAKE_CXX_STANDARD_LIBRARIES "-lSDL2 -lGLEW -lpthread -lz -ldl -lGL -lrt -lpng -lGLU")


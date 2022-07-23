cmake_minimum_required(VERSION 3.10)

project(GameOfLife)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -O3")

add_executable(gameoflife main.cpp gameoflife.cpp herbivore.cpp herd.cpp x11_display.cpp fps_clock.cpp)
target_link_libraries(gameoflife X11)
target_include_directories(gameoflife PRIVATE /usr/X11/include)

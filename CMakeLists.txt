cmake_minimum_required(VERSION 3.16)
project(tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tetris
    src/main.cpp
    src/Tetromino.cpp
    src/Board.cpp
    src/Game.cpp
    src/SoundManager.cpp
    src/Console/ConsoleRenderer.cpp
    src/Console/ConsoleInput.cpp
)

target_include_directories(tetris PRIVATE include)

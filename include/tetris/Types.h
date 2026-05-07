#pragma once

#include <cstdint>

namespace tetris {

// Cell colors
enum class Color : uint8_t {
    EMPTY = 0,
    CYAN,    // I
    YELLOW,  // O
    PURPLE,  // T
    BLUE,    // J
    ORANGE,  // L
    GREEN,   // S
    RED,     // Z
    WHITE    // For flash effects
};

// Tetromino types
enum class TetrominoType : uint8_t {
    I = 0,
    O,
    T,
    L,
    J,
    S,
    Z,
    COUNT  // 7 total
};

// Game states
enum class GameState : uint8_t {
    MENU,
    PLAYING,
    GAME_OVER
};

} // namespace tetris

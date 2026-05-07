#include "tetris/Tetromino.h"
#include "tetris/Constants.h"
#include <stdexcept>

namespace tetris {

// Shape definitions based on user's specification
// Each shape is stored as 4 rows of 4 bits (MSB = leftmost)
//
// I: 1111    O: 11     T: 111    L: 1      J:  1     S:  11    Z: 11
//                 11          1          1          1          11         11
//                                    11                         11

const std::array<Tetromino::RotationStates, 7> Tetromino::SHAPES = {{
    // I - Cyan
    {{
        {0b1111, 0b0000, 0b0000, 0b0000},  // Horizontal
        {0b0010, 0b0010, 0b0010, 0b0010},  // Vertical
        {0b0000, 0b0000, 0b1111, 0b0000},  // Horizontal (shifted)
        {0b0100, 0b0100, 0b0100, 0b0100},  // Vertical (shifted)
    }},
    // O - Yellow
    {{
        {0b1100, 0b1100, 0b0000, 0b0000},
        {0b1100, 0b1100, 0b0000, 0b0000},
        {0b1100, 0b1100, 0b0000, 0b0000},
        {0b1100, 0b1100, 0b0000, 0b0000},
    }},
    // T - Purple
    {{
        {0b1110, 0b0100, 0b0000, 0b0000},  // Flat top
        {0b0100, 0b1100, 0b0100, 0b0000},  // Right
        {0b0000, 0b0100, 0b1110, 0b0000},  // Flat bottom
        {0b0100, 0b0110, 0b0100, 0b0000},  // Left
    }},
    // L - Orange
    {{
        {0b0010, 0b0010, 0b0110, 0b0000},  // Vertical with foot right
        {0b0000, 0b1110, 0b1000, 0b0000},  // Horizontal with foot left
        {0b1100, 0b1000, 0b1000, 0b0000},  // Vertical with foot left
        {0b0010, 0b1110, 0b0000, 0b0000},  // Horizontal with foot right
    }},
    // J - Blue
    {{
        {0b0100, 0b0100, 0b0110, 0b0000},  // Vertical with foot left
        {0b0000, 0b1110, 0b0010, 0b0000},  // Horizontal with foot right
        {0b1100, 0b0100, 0b0100, 0b0000},  // Vertical with foot right
        {0b1000, 0b1110, 0b0000, 0b0000},  // Horizontal with foot left
    }},
    // S - Green
    {{
        {0b0110, 0b1100, 0b0000, 0b0000},  // Horizontal
        {0b0100, 0b0110, 0b0010, 0b0000},  // Vertical
        {0b0000, 0b0110, 0b1100, 0b0000},  // Horizontal (shifted)
        {0b1000, 0b1100, 0b0100, 0b0000},  // Vertical (shifted)
    }},
    // Z - Red
    {{
        {0b1100, 0b0110, 0b0000, 0b0000},  // Horizontal
        {0b0010, 0b0110, 0b0100, 0b0000},  // Vertical
        {0b0000, 0b1100, 0b0110, 0b0000},  // Horizontal (shifted)
        {0b0100, 0b1100, 0b1000, 0b0000},  // Vertical (shifted)
    }},
}};

const std::array<Color, 7> Tetromino::COLORS = {{
    Color::CYAN,    // I
    Color::YELLOW,  // O
    Color::PURPLE,  // T
    Color::ORANGE,  // L
    Color::BLUE,    // J
    Color::GREEN,   // S
    Color::RED,     // Z
}};

// Spawn positions (centered horizontally, at top)
const std::array<int, 7> Tetromino::SPAWN_X = {{
    3,  // I
    4,  // O
    3,  // T
    3,  // L
    3,  // J
    3,  // S
    3,  // Z
}};

const std::array<int, 7> Tetromino::SPAWN_Y = {{
    0,  // I
    0,  // O
    0,  // T
    0,  // L
    0,  // J
    0,  // S
    0,  // Z
}};

Tetromino::Tetromino(TetrominoType type)
    : type_(type)
    , color_(COLORS[static_cast<int>(type)])
    , rotation_(0)
    , x_(SPAWN_X[static_cast<int>(type)])
    , y_(SPAWN_Y[static_cast<int>(type)])
{
}

const Tetromino::Shape& Tetromino::getCurrentShape() const {
    return SHAPES[static_cast<int>(type_)][rotation_];
}

const Tetromino::RotationStates& Tetromino::getAllRotations() const {
    return SHAPES[static_cast<int>(type_)];
}

void Tetromino::rotate() {
    rotation_ = (rotation_ + 1) % 4;
}

void Tetromino::rotateCounterClockwise() {
    rotation_ = (rotation_ + 3) % 4;  // +3 is same as -1 mod 4
}

void Tetromino::moveLeft() {
    x_--;
}

void Tetromino::moveRight() {
    x_++;
}

void Tetromino::moveDown() {
    y_++;
}

void Tetromino::moveUp() {
    y_--;
}

int Tetromino::getSpawnX() const {
    return SPAWN_X[static_cast<int>(type_)];
}

int Tetromino::getSpawnY() const {
    return SPAWN_Y[static_cast<int>(type_)];
}

void Tetromino::reset() {
    rotation_ = 0;
    x_ = getSpawnX();
    y_ = getSpawnY();
}

} // namespace tetris

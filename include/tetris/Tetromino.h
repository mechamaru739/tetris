#pragma once

#include "Types.h"
#include <array>
#include <cstdint>

namespace tetris {

class Tetromino {
public:
    using ShapeRow = uint8_t;
    using Shape = std::array<ShapeRow, 4>;
    using RotationStates = std::array<Shape, 4>;

    Tetromino(TetrominoType type);

    // Shape access
    const Shape& getCurrentShape() const;
    const RotationStates& getAllRotations() const;

    // Rotation
    void rotate();
    void rotateCounterClockwise();

    // Movement
    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();

    // Position
    int getX() const { return x_; }
    int getY() const { return y_; }
    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }

    // Type info
    TetrominoType getType() const { return type_; }
    Color getColor() const { return color_; }
    int getRotation() const { return rotation_; }

    // Spawn position
    int getSpawnX() const;
    int getSpawnY() const;

    // Reset to spawn state
    void reset();

private:
    TetrominoType type_;
    Color color_;
    int rotation_;
    int x_, y_;

    // Static data for all tetromino shapes
    static const std::array<RotationStates, 7> SHAPES;
    static const std::array<Color, 7> COLORS;
    static const std::array<int, 7> SPAWN_X;
    static const std::array<int, 7> SPAWN_Y;
};

} // namespace tetris

#pragma once

#include "Types.h"
#include "Tetromino.h"
#include <vector>

namespace tetris {

class Board {
public:
    Board(int width = BOARD_WIDTH, int height = BOARD_HEIGHT);

    // Collision detection
    bool canPlace(const Tetromino& tetromino) const;

    // Place tetromino on board
    void place(const Tetromino& tetromino);

    // Clear completed lines, returns number of lines cleared
    int clearLines();

    // Get full lines (for flash effect)
    std::vector<int> getFullLines() const;

    // Clear specific lines
    void clearSpecificLines(const std::vector<int>& lines);

    // Grid access
    bool isOccupied(int x, int y) const;
    Color getColor(int x, int y) const;

    // Reset board to empty
    void reset();

    // Getters
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    int width_;
    int height_;
    std::vector<std::vector<Color>> grid_;

    bool isLineFull(int y) const;
    void removeLine(int y);

    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;
};

} // namespace tetris

#include "tetris/Board.h"
#include "tetris/Constants.h"
#include <algorithm>

namespace tetris {

Board::Board(int width, int height)
    : width_(width)
    , height_(height)
    , grid_(height, std::vector<Color>(width, Color::EMPTY))
{
}

bool Board::canPlace(const Tetromino& tetromino) const {
    const auto& shape = tetromino.getCurrentShape();
    int tx = tetromino.getX();
    int ty = tetromino.getY();

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            // Check if this cell is occupied in the tetromino
            if (shape[y] & (1 << (3 - x))) {
                int boardX = tx + x;
                int boardY = ty + y;

                // Boundary check
                if (boardX < 0 || boardX >= width_)
                    return false;
                if (boardY < 0 || boardY >= height_)
                    return false;

                // Collision with existing pieces
                if (grid_[boardY][boardX] != Color::EMPTY)
                    return false;
            }
        }
    }
    return true;
}

void Board::place(const Tetromino& tetromino) {
    const auto& shape = tetromino.getCurrentShape();
    int tx = tetromino.getX();
    int ty = tetromino.getY();
    Color color = tetromino.getColor();

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (shape[y] & (1 << (3 - x))) {
                int boardX = tx + x;
                int boardY = ty + y;

                if (boardX >= 0 && boardX < width_ &&
                    boardY >= 0 && boardY < height_) {
                    grid_[boardY][boardX] = color;
                }
            }
        }
    }
}

int Board::clearLines() {
    int linesCleared = 0;

    // Scan from bottom to top
    for (int y = height_ - 1; y >= 0; --y) {
        if (isLineFull(y)) {
            removeLine(y);
            linesCleared++;
            y++;  // Re-check this position since lines shifted down
        }
    }

    return linesCleared;
}

std::vector<int> Board::getFullLines() const {
    std::vector<int> fullLines;
    for (int y = 0; y < height_; ++y) {
        if (isLineFull(y)) {
            fullLines.push_back(y);
        }
    }
    return fullLines;
}

void Board::clearSpecificLines(const std::vector<int>& lines) {
    // Clear lines from bottom to top to maintain correct indices
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        removeLine(*it);
    }
}

bool Board::isOccupied(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return false;
    return grid_[y][x] != Color::EMPTY;
}

Color Board::getColor(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return Color::EMPTY;
    return grid_[y][x];
}

void Board::reset() {
    for (auto& row : grid_) {
        std::fill(row.begin(), row.end(), Color::EMPTY);
    }
}

bool Board::isLineFull(int y) const {
    for (int x = 0; x < width_; ++x) {
        if (grid_[y][x] == Color::EMPTY)
            return false;
    }
    return true;
}

void Board::removeLine(int y) {
    // Shift all lines above down by one
    for (int row = y; row > 0; --row) {
        grid_[row] = grid_[row - 1];
    }
    // Clear top line
    std::fill(grid_[0].begin(), grid_[0].end(), Color::EMPTY);
}

} // namespace tetris

#pragma once

#include "Board.h"
#include "Tetromino.h"
#include <memory>

namespace tetris {

class Renderer {
public:
    virtual ~Renderer() = default;

    // Clear the screen/buffer
    virtual void clear() = 0;

    // Render the game board
    virtual void renderBoard(const Board& board) = 0;

    // Render the current falling tetromino
    virtual void renderTetromino(const Tetromino& tetromino) = 0;

    // Render the next piece preview
    virtual void renderNextPiece(const Tetromino& next) = 0;

    // Render flash effect for lines being cleared
    virtual void renderFlashLines(const Board& board, const std::vector<int>& lines, int flashFrame) = 0;

    // Render score and lines
    virtual void renderScore(int score, int lines) = 0;

    // Render mute status
    virtual void renderMuteStatus(bool isMuted) = 0;

    // Render game over screen
    virtual void renderGameOver(int finalScore, int finalLines) = 0;

    // Render menu screen
    virtual void renderMenu() = 0;

    // Present the buffer to screen
    virtual void present() = 0;
};

} // namespace tetris

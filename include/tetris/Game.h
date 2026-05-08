#pragma once

#include "Board.h"
#include "Tetromino.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "SoundManager.h"
#include "Types.h"
#include <memory>

namespace tetris {

class Game {
public:
    Game(std::unique_ptr<Renderer> renderer, std::unique_ptr<InputHandler> input);

    // Main game loop
    void run();

private:
    GameState state_;
    Board board_;

    std::unique_ptr<Tetromino> current_;
    std::unique_ptr<Tetromino> next_;

    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<InputHandler> input_;
    SoundManager soundManager_;

    int score_;
    int linesCleared_;
    int dropTimer_;

    // Line clear animation
    bool isClearingLines_;
    std::vector<int> linesToClear_;
    int flashTimer_;
    int flashFrame_;
    static constexpr int FLASH_DURATION = 300;  // ms
    static constexpr int FLASH_FRAMES = 6;

    // Game logic methods
    void spawnTetromino();
    void handleMenuInput();
    void handleGameInput();
    void handleGameOverInput();
    void update();
    void updateLineClear();
    void render();
    void lockPiece();
    bool isGameOver() const;

    // Scoring
    void addScore(int lines);

    // Random tetromino generation
    TetrominoType getRandomType();
};

} // namespace tetris

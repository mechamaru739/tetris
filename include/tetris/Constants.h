#pragma once

namespace tetris {

// Board dimensions
constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 20;

// Timing
constexpr int DROP_INTERVAL = 500;  // ms between automatic drops
constexpr int FRAME_TIME = 16;      // ~60 FPS (ms)

// Scoring
constexpr int SCORE_SINGLE = 100;
constexpr int SCORE_DOUBLE = 300;
constexpr int SCORE_TRIPLE = 500;
constexpr int SCORE_TETRIS = 800;

// Display
constexpr int CELL_WIDTH = 2;  // Each cell is 2 characters wide

// Preview box dimensions
constexpr int PREVIEW_WIDTH = 6;
constexpr int PREVIEW_HEIGHT = 5;

} // namespace tetris

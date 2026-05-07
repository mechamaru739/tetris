#include "tetris/Game.h"
#include "tetris/Constants.h"
#include <chrono>
#include <thread>
#include <random>

namespace tetris {

Game::Game(std::unique_ptr<Renderer> renderer, std::unique_ptr<InputHandler> input)
    : state_(GameState::MENU)
    , board_()
    , current_(nullptr)
    , next_(nullptr)
    , renderer_(std::move(renderer))
    , input_(std::move(input))
    , score_(0)
    , linesCleared_(0)
    , dropTimer_(0)
    , isClearingLines_(false)
    , flashTimer_(0)
    , flashFrame_(0)
{
}

void Game::run() {
    using Clock = std::chrono::steady_clock;
    constexpr auto FRAME_TIME_MS = std::chrono::milliseconds(FRAME_TIME);

    while (true) {
        auto frameStart = Clock::now();

        switch (state_) {
            case GameState::MENU:
                handleMenuInput();
                render();
                break;

            case GameState::PLAYING:
                handleGameInput();
                update();
                render();
                break;

            case GameState::GAME_OVER:
                handleGameOverInput();
                render();
                break;
        }

        // Frame rate limiting
        auto elapsed = Clock::now() - frameStart;
        if (elapsed < FRAME_TIME_MS) {
            std::this_thread::sleep_for(FRAME_TIME_MS - elapsed);
        }
    }
}

void Game::spawnTetromino() {
    if (!next_) {
        next_ = std::make_unique<Tetromino>(getRandomType());
    }

    current_ = std::move(next_);
    next_ = std::make_unique<Tetromino>(getRandomType());

    // Check game over
    if (!board_.canPlace(*current_)) {
        state_ = GameState::GAME_OVER;
    }
}

void Game::handleMenuInput() {
    InputEvents events = input_->getEvents();

    if (events.enterPressed) {
        state_ = GameState::PLAYING;
        board_.reset();
        score_ = 0;
        linesCleared_ = 0;
        dropTimer_ = 0;
        current_ = nullptr;
        next_ = nullptr;
        spawnTetromino();
    } else if (events.escapePressed) {
        exit(0);
    }
}

void Game::handleGameInput() {
    // Don't handle input during line clear animation
    if (isClearingLines_) return;

    InputEvents events = input_->getEvents();

    if (!current_) return;

    // Rotation
    if (events.rotatePressed) {
        current_->rotate();
        if (!board_.canPlace(*current_)) {
            // Try wall kicks
            int originalX = current_->getX();

            // Try moving left
            current_->setX(originalX - 1);
            if (board_.canPlace(*current_)) return;

            // Try moving right
            current_->setX(originalX + 1);
            if (board_.canPlace(*current_)) return;

            // Try moving right 2 (for I piece)
            current_->setX(originalX + 2);
            if (board_.canPlace(*current_)) return;

            // Revert rotation
            current_->setX(originalX);
            current_->rotateCounterClockwise();
        }
    }

    // Left movement
    if (events.leftPressed) {
        current_->moveLeft();
        if (!board_.canPlace(*current_)) {
            current_->moveRight();
        }
    }

    // Right movement
    if (events.rightPressed) {
        current_->moveRight();
        if (!board_.canPlace(*current_)) {
            current_->moveLeft();
        }
    }

    // Soft drop
    if (events.downHeld) {
        current_->moveDown();
        if (!board_.canPlace(*current_)) {
            current_->moveUp();
            lockPiece();
        }
    }

    // Hard drop
    if (events.dropPressed) {
        while (board_.canPlace(*current_)) {
            current_->moveDown();
        }
        current_->moveUp();
        lockPiece();
    }
}

void Game::handleGameOverInput() {
    InputEvents events = input_->getEvents();

    if (events.enterPressed) {
        state_ = GameState::PLAYING;
        board_.reset();
        score_ = 0;
        linesCleared_ = 0;
        dropTimer_ = 0;
        current_ = nullptr;
        next_ = nullptr;
        spawnTetromino();
    } else if (events.escapePressed) {
        exit(0);
    }
}

void Game::update() {
    // Handle line clear animation
    if (isClearingLines_) {
        updateLineClear();
        return;
    }

    if (!current_) return;

    dropTimer_ += FRAME_TIME;

    if (dropTimer_ >= DROP_INTERVAL) {
        dropTimer_ = 0;

        current_->moveDown();
        if (!board_.canPlace(*current_)) {
            current_->moveUp();
            lockPiece();
        }
    }
}

void Game::updateLineClear() {
    flashTimer_ += FRAME_TIME;

    if (flashTimer_ >= FLASH_DURATION / FLASH_FRAMES) {
        flashTimer_ = 0;
        flashFrame_++;

        if (flashFrame_ >= FLASH_FRAMES) {
            // Animation complete, clear the lines
            board_.clearSpecificLines(linesToClear_);
            addScore(static_cast<int>(linesToClear_.size()));
            linesToClear_.clear();
            isClearingLines_ = false;
            flashFrame_ = 0;
            spawnTetromino();
        }
    }
}

void Game::render() {
    renderer_->clear();

    switch (state_) {
        case GameState::MENU:
            renderer_->renderMenu();
            break;

        case GameState::PLAYING:
            renderer_->renderBoard(board_);
            if (isClearingLines_) {
                renderer_->renderFlashLines(board_, linesToClear_, flashFrame_);
            }
            if (current_ && !isClearingLines_) {
                renderer_->renderTetromino(*current_);
            }
            if (next_) {
                renderer_->renderNextPiece(*next_);
            }
            renderer_->renderScore(score_, linesCleared_);
            break;

        case GameState::GAME_OVER:
            renderer_->renderBoard(board_);
            renderer_->renderGameOver(score_, linesCleared_);
            break;
    }

    renderer_->present();
}

void Game::lockPiece() {
    board_.place(*current_);

    // Check for full lines
    linesToClear_ = board_.getFullLines();

    if (!linesToClear_.empty()) {
        // Start line clear animation
        isClearingLines_ = true;
        flashTimer_ = 0;
        flashFrame_ = 0;
    } else {
        // No lines to clear, spawn next piece immediately
        spawnTetromino();
    }
}

bool Game::isGameOver() const {
    return !board_.canPlace(*current_);
}

void Game::addScore(int lines) {
    linesCleared_ += lines;

    switch (lines) {
        case 1: score_ += SCORE_SINGLE; break;
        case 2: score_ += SCORE_DOUBLE; break;
        case 3: score_ += SCORE_TRIPLE; break;
        case 4: score_ += SCORE_TETRIS; break;
    }
}

TetrominoType Game::getRandomType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, static_cast<int>(TetrominoType::COUNT) - 1);
    return static_cast<TetrominoType>(dis(gen));
}

} // namespace tetris

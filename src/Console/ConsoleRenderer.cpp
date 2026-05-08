#include "tetris/Console/ConsoleRenderer.h"
#include "tetris/Constants.h"
#include <sstream>
#include <iomanip>

namespace tetris {

ConsoleRenderer::ConsoleRenderer()
    : hConsole_(INVALID_HANDLE_VALUE)
    , bufferSize_({SCREEN_WIDTH, SCREEN_HEIGHT})
    , bufferCoord_({0, 0})
    , writeRegion_({0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1})
{
    initConsole();
    buffer_.resize(SCREEN_WIDTH * SCREEN_HEIGHT);
}

ConsoleRenderer::~ConsoleRenderer() {
}

void ConsoleRenderer::initConsole() {
    hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set UTF-8 code page
    SetConsoleOutputCP(65001);

    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole_, &cursorInfo);

    // Set console title
    SetConsoleTitleA("Tetris");

    // Set window size
    SMALL_RECT windowSize = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
    SetConsoleWindowInfo(hConsole_, TRUE, &windowSize);

    // Set buffer size
    SetConsoleScreenBufferSize(hConsole_, bufferSize_);
}

void ConsoleRenderer::clear() {
    for (auto& cell : buffer_) {
        cell.Char.UnicodeChar = L' ';
        cell.Attributes = 0;
    }
}

void ConsoleRenderer::renderBoard(const Board& board) {
    WORD borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    // Top border
    setCell(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y - 1, L'+', borderColor);
    for (int x = 0; x < board.getWidth() * CELL_WIDTH; ++x) {
        setCell(BOARD_OFFSET_X + x, BOARD_OFFSET_Y - 1, L'-', borderColor);
    }
    setCell(BOARD_OFFSET_X + board.getWidth() * CELL_WIDTH, BOARD_OFFSET_Y - 1, L'+', borderColor);

    // Side borders and board content
    for (int y = 0; y < board.getHeight(); ++y) {
        setCell(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y + y, L'|', borderColor);
        setCell(BOARD_OFFSET_X + board.getWidth() * CELL_WIDTH, BOARD_OFFSET_Y + y, L'|', borderColor);

        for (int x = 0; x < board.getWidth(); ++x) {
            Color color = board.getColor(x, y);
            WORD attr = getColorAttribute(color);

            if (color != Color::EMPTY) {
                setCell(BOARD_OFFSET_X + x * CELL_WIDTH, BOARD_OFFSET_Y + y, L'[', attr);
                setCell(BOARD_OFFSET_X + x * CELL_WIDTH + 1, BOARD_OFFSET_Y + y, L']', attr);
            } else {
                setCell(BOARD_OFFSET_X + x * CELL_WIDTH, BOARD_OFFSET_Y + y, L'.',
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                setCell(BOARD_OFFSET_X + x * CELL_WIDTH + 1, BOARD_OFFSET_Y + y, L' ',
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
    }

    // Bottom border
    setCell(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y + board.getHeight(), L'+', borderColor);
    for (int x = 0; x < board.getWidth() * CELL_WIDTH; ++x) {
        setCell(BOARD_OFFSET_X + x, BOARD_OFFSET_Y + board.getHeight(), L'-', borderColor);
    }
    setCell(BOARD_OFFSET_X + board.getWidth() * CELL_WIDTH, BOARD_OFFSET_Y + board.getHeight(), L'+', borderColor);
}

void ConsoleRenderer::renderTetromino(const Tetromino& tetromino) {
    const auto& shape = tetromino.getCurrentShape();
    int tx = tetromino.getX();
    int ty = tetromino.getY();
    WORD attr = getColorAttribute(tetromino.getColor());

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (shape[y] & (1 << (3 - x))) {
                int screenX = BOARD_OFFSET_X + (tx + x) * CELL_WIDTH;
                int screenY = BOARD_OFFSET_Y + ty + y;

                if (screenX >= BOARD_OFFSET_X &&
                    screenX < BOARD_OFFSET_X + BOARD_WIDTH * CELL_WIDTH &&
                    screenY >= BOARD_OFFSET_Y &&
                    screenY < BOARD_OFFSET_Y + BOARD_HEIGHT) {
                    setCell(screenX, screenY, L'[', attr);
                    setCell(screenX + 1, screenY, L']', attr);
                }
            }
        }
    }
}

void ConsoleRenderer::renderNextPiece(const Tetromino& next) {
    drawNextPieceBox();

    const auto& shape = next.getCurrentShape();
    WORD attr = getColorAttribute(next.getColor());

    int previewX = INFO_OFFSET_X + 2;
    int previewY = INFO_OFFSET_Y + 3;

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (shape[y] & (1 << (3 - x))) {
                setCell(previewX + x * CELL_WIDTH, previewY + y, L'[', attr);
                setCell(previewX + x * CELL_WIDTH + 1, previewY + y, L']', attr);
            }
        }
    }
}

void ConsoleRenderer::renderFlashLines(const Board& board, const std::vector<int>& lines, int flashFrame) {
    // Flash effect: alternate between white and original color
    WORD flashColor;
    if (flashFrame % 2 == 0) {
        flashColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    } else {
        flashColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
    }

    for (int y : lines) {
        for (int x = 0; x < board.getWidth(); ++x) {
            if (flashFrame % 2 == 0) {
                // White flash
                setCell(BOARD_OFFSET_X + x * CELL_WIDTH, BOARD_OFFSET_Y + y, L'[', flashColor);
                setCell(BOARD_OFFSET_X + x * CELL_WIDTH + 1, BOARD_OFFSET_Y + y, L']', flashColor);
            } else {
                // Original color or empty
                Color color = board.getColor(x, y);
                WORD attr = getColorAttribute(color);
                if (color != Color::EMPTY) {
                    setCell(BOARD_OFFSET_X + x * CELL_WIDTH, BOARD_OFFSET_Y + y, L'[', attr);
                    setCell(BOARD_OFFSET_X + x * CELL_WIDTH + 1, BOARD_OFFSET_Y + y, L']', attr);
                }
            }
        }
    }
}

void ConsoleRenderer::renderScore(int score, int lines) {
    drawScoreBox();

    WORD textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    std::wstringstream scoreStr;
    scoreStr << std::setw(8) << score;
    drawString(INFO_OFFSET_X + 2, INFO_OFFSET_Y + 11, scoreStr.str(), textColor);

    std::wstringstream linesStr;
    linesStr << std::setw(8) << lines;
    drawString(INFO_OFFSET_X + 2, INFO_OFFSET_Y + 14, linesStr.str(), textColor);
}

void ConsoleRenderer::renderGameOver(int finalScore, int finalLines) {
    WORD titleColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
    WORD textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD dimColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    drawBox(centerX - 12, centerY - 5, 24, 10, titleColor);

    drawString(centerX - 10, centerY - 3, L"    GAME OVER!    ", titleColor);

    std::wstringstream scoreStr;
    scoreStr << L"Score: " << std::setw(8) << finalScore;
    drawString(centerX - 10, centerY - 1, scoreStr.str(), textColor);

    std::wstringstream linesStr;
    linesStr << L"Lines: " << std::setw(8) << finalLines;
    drawString(centerX - 10, centerY, linesStr.str(), textColor);

    drawString(centerX - 10, centerY + 2, L" ENTER: Restart ", textColor);
    drawString(centerX - 10, centerY + 3, L" ESC:   Quit    ", dimColor);
}

void ConsoleRenderer::renderMenu() {
    WORD titleColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    WORD textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD dimColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    // ASCII art title
    drawString(centerX - 9, centerY - 6, L"  ####  ####  ####", titleColor);
    drawString(centerX - 9, centerY - 5, L"    ##  ####  ##  ", titleColor);
    drawString(centerX - 9, centerY - 4, L"   ##    ##   ##  ", titleColor);
    drawString(centerX - 9, centerY - 3, L"  ####  ####  ####", titleColor);

    drawString(centerX - 3, centerY - 1, L"TETRIS", titleColor);

    drawString(centerX - 10, centerY + 2, L" ENTER: Start   ", textColor);
    drawString(centerX - 10, centerY + 3, L" ESC:   Quit    ", dimColor);

    drawString(centerX - 12, centerY + 6, L" Arrows: Move/Rotate", dimColor);
    drawString(centerX - 12, centerY + 7, L" Space:  Hard Drop  ", dimColor);
}

void ConsoleRenderer::present() {
    WriteConsoleOutput(
        hConsole_,
        buffer_.data(),
        bufferSize_,
        bufferCoord_,
        &writeRegion_
    );
}

void ConsoleRenderer::setCell(int x, int y, wchar_t ch, WORD attributes) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        int index = y * SCREEN_WIDTH + x;
        buffer_[index].Char.UnicodeChar = ch;
        buffer_[index].Attributes = attributes;
    }
}

void ConsoleRenderer::drawString(int x, int y, const std::wstring& str, WORD attributes) {
    for (size_t i = 0; i < str.size(); ++i) {
        setCell(x + static_cast<int>(i), y, str[i], attributes);
    }
}

void ConsoleRenderer::drawBox(int x, int y, int width, int height, WORD attributes) {
    setCell(x, y, L'+', attributes);
    for (int i = 1; i < width - 1; ++i) {
        setCell(x + i, y, L'-', attributes);
    }
    setCell(x + width - 1, y, L'+', attributes);

    for (int i = 1; i < height - 1; ++i) {
        setCell(x, y + i, L'|', attributes);
        setCell(x + width - 1, y + i, L'|', attributes);
    }

    setCell(x, y + height - 1, L'+', attributes);
    for (int i = 1; i < width - 1; ++i) {
        setCell(x + i, y + height - 1, L'-', attributes);
    }
    setCell(x + width - 1, y + height - 1, L'+', attributes);
}

WORD ConsoleRenderer::getColorAttribute(Color color) const {
    switch (color) {
        case Color::CYAN:   return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::PURPLE: return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case Color::BLUE:   return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case Color::ORANGE: return FOREGROUND_RED | FOREGROUND_GREEN;
        case Color::GREEN:  return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::RED:    return FOREGROUND_RED | FOREGROUND_INTENSITY;
        case Color::WHITE:  return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        default:            return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

void ConsoleRenderer::drawNextPieceBox() {
    WORD borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    WORD textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    drawBox(INFO_OFFSET_X, INFO_OFFSET_Y, PREVIEW_WIDTH + 4, PREVIEW_HEIGHT + 4, borderColor);
    drawString(INFO_OFFSET_X + 2, INFO_OFFSET_Y + 1, L"NEXT:", textColor);
}

void ConsoleRenderer::drawScoreBox() {
    WORD borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    WORD textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    drawBox(INFO_OFFSET_X, INFO_OFFSET_Y + 8, PREVIEW_WIDTH + 4, 9, borderColor);
    drawString(INFO_OFFSET_X + 2, INFO_OFFSET_Y + 9, L"SCORE:", textColor);
    drawString(INFO_OFFSET_X + 2, INFO_OFFSET_Y + 12, L"LINES:", textColor);
}

void ConsoleRenderer::renderMuteStatus(bool isMuted) {
    WORD textColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD dimColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    if (isMuted) {
        drawString(INFO_OFFSET_X, INFO_OFFSET_Y + 20, L"M: Sound OFF", dimColor);
    } else {
        drawString(INFO_OFFSET_X, INFO_OFFSET_Y + 20, L"M: Sound ON ", textColor);
    }
}

} // namespace tetris

#pragma once

#include "tetris/Renderer.h"
#include <windows.h>
#include <vector>
#include <string>

namespace tetris {

class ConsoleRenderer : public Renderer {
public:
    ConsoleRenderer();
    ~ConsoleRenderer() override;

    // Renderer interface
    void clear() override;
    void renderBoard(const Board& board) override;
    void renderTetromino(const Tetromino& tetromino) override;
    void renderNextPiece(const Tetromino& next) override;
    void renderFlashLines(const Board& board, const std::vector<int>& lines, int flashFrame) override;
    void renderScore(int score, int lines) override;
    void renderMuteStatus(bool isMuted) override;
    void renderGameOver(int finalScore, int finalLines) override;
    void renderMenu() override;
    void present() override;

private:
    HANDLE hConsole_;
    std::vector<CHAR_INFO> buffer_;
    COORD bufferSize_;
    COORD bufferCoord_;
    SMALL_RECT writeRegion_;

    // Layout constants
    static constexpr int SCREEN_WIDTH = 40;
    static constexpr int SCREEN_HEIGHT = 25;
    static constexpr int BOARD_OFFSET_X = 2;
    static constexpr int BOARD_OFFSET_Y = 2;
    static constexpr int INFO_OFFSET_X = 26;
    static constexpr int INFO_OFFSET_Y = 2;

    // Helper methods
    void initConsole();
    void setCell(int x, int y, wchar_t ch, WORD attributes);
    void drawString(int x, int y, const std::wstring& str, WORD attributes);
    void drawBox(int x, int y, int width, int height, WORD attributes);
    WORD getColorAttribute(Color color) const;
    void drawNextPieceBox();
    void drawScoreBox();
};

} // namespace tetris

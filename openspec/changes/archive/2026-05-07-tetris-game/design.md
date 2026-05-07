# Technical Design

## Architecture Overview

The Tetris game follows a clean architecture with separation of concerns:

```
┌─────────────────────────────────────────────────────────────┐
│                    Architecture Layers                       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   ┌─────────────────────────────────────────────────────┐  │
│   │                  Presentation Layer                  │  │
│   │  ┌──────────────┐  ┌──────────────┐  ┌───────────┐  │  │
│   │  │ConsoleRenderer│  │ ConsoleInput │  │  Screen   │  │  │
│   │  └──────────────┘  └──────────────┘  └───────────┘  │  │
│   └─────────────────────────────────────────────────────┘  │
│                            │                               │
│                            ▼                               │
│   ┌─────────────────────────────────────────────────────┐  │
│   │                   Application Layer                  │  │
│   │  ┌──────────────┐  ┌──────────────┐  ┌───────────┐  │  │
│   │  │     Game     │  │   Board      │  │ Tetromino │  │  │
│   │  └──────────────┘  └──────────────┘  └───────────┘  │  │
│   └─────────────────────────────────────────────────────┘  │
│                            │                               │
│                            ▼                               │
│   ┌─────────────────────────────────────────────────────┐  │
│   │                    Platform Layer                    │  │
│   │  ┌──────────────────────────────────────────────┐   │  │
│   │  │         Windows Console API                   │   │  │
│   │  │  - WriteConsoleOutput                         │   │  │
│   │  │  - GetAsyncKeyState                           │   │  │
│   │  │  - SetConsoleCursorPosition                   │   │  │
│   │  └──────────────────────────────────────────────┘   │  │
│   └─────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## Data Structures

### Tetromino Representation

Each tetromino is stored as a 4x4 bit mask array representing 4 rotation states:

```cpp
// Example: T-piece rotations
Rotation 0 (spawn):    Rotation 1 (90°):    Rotation 2 (180°):   Rotation 3 (270°):
111                    01                   000                   10
010                    11                   010                   11
000                    01                   011                   10
000                    00                   000                   00

Bit representation:
Row 0: 0b0111 = 7     Row 0: 0b0010 = 2    Row 0: 0b0000 = 0    Row 0: 0b0100 = 4
Row 1: 0b0010 = 2     Row 1: 0b0110 = 6    Row 1: 0b0010 = 2    Row 1: 0b0110 = 6
Row 2: 0b0000 = 0     Row 2: 0b0010 = 2    Row 2: 0b0110 = 6    Row 2: 0b0100 = 4
Row 3: 0b0000 = 0     Row 3: 0b0000 = 0    Row 3: 0b0000 = 0    Row 3: 0b0000 = 0
```

### Board Representation

```cpp
// 10x20 grid storing color values
// Color::EMPTY (0) for empty cells
// Color::CYAN (1-7) for different piece colors
std::vector<std::vector<Color>> grid;

// Access pattern:
// grid[y][x] where y=0 is top, y=19 is bottom
```

### Shape Definition Table

All 7 tetrominoes with 4 rotations each, stored as static constexpr:

```cpp
struct TetrominoData {
    std::array<std::array<uint8_t, 4>, 4> rotations;
    Color color;
    int spawnX;  // Center spawn position
    int spawnY;
};

static constexpr std::array<TetrominoData, 7> TETROMINO_TABLE = {{
    // I-piece
    {{
        {0b0000, 0b1111, 0b0000, 0b0000},  // Rotation 0
        {0b0010, 0b0010, 0b0010, 0b0010},  // Rotation 1
        {0b0000, 0b0000, 0b1111, 0b0000},  // Rotation 2
        {0b0100, 0b0100, 0b0100, 0b0100},  // Rotation 3
    }},
    Color::CYAN,
    3, 0  // Spawn at (3, 0)
    // ... other pieces
}};
```

## Collision Detection Algorithm

```cpp
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
                if (boardX < 0 || boardX >= BOARD_WIDTH)
                    return false;
                if (boardY < 0 || boardY >= BOARD_HEIGHT)
                    return false;

                // Collision with existing pieces
                if (grid_[boardY][boardX] != Color::EMPTY)
                    return false;
            }
        }
    }
    return true;
}
```

## Rendering Strategy

### Double Buffering

To prevent flickering, we use a back buffer that we write to, then swap:

```
┌─────────────────┐         ┌─────────────────┐
│   Back Buffer   │  swap   │  Front Buffer   │
│   (drawing)     │────────▶│   (display)     │
└─────────────────┘         └─────────────────┘
```

Implementation using Windows Console API:

```cpp
class ConsoleRenderer : public Renderer {
private:
    HANDLE hConsole_;
    std::vector<CHAR_INFO> backBuffer_;
    COORD bufferSize_;
    COORD bufferCoord_;
    SMALL_RECT writeRegion_;

    void present() override {
        // Write entire buffer to console at once
        WriteConsoleOutput(
            hConsole_,
            backBuffer_.data(),
            bufferSize_,
            bufferCoord_,
            &writeRegion_
        );
    }
};
```

### Character Rendering

Each board cell is rendered as 2 characters wide for better aspect ratio:

```
Empty cell: "  " (two spaces)
Filled cell: "██" (two full-block characters)
Border: "│", "─", "┌", "┐", "└", "┘"
```

### Color Mapping

```cpp
WORD getColorAttribute(Color color) {
    switch (color) {
        case Color::CYAN:   return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::PURPLE: return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case Color::BLUE:   return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case Color::ORANGE: return FOREGROUND_RED | FOREGROUND_GREEN;
        case Color::GREEN:  return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::RED:    return FOREGROUND_RED | FOREGROUND_INTENSITY;
        default:            return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}
```

## Game Loop Design

```cpp
void Game::run() {
    using Clock = std::chrono::steady_clock;
    constexpr auto FRAME_TIME = std::chrono::milliseconds(16); // ~60 FPS

    while (true) {
        auto frameStart = Clock::now();

        switch (state_) {
            case State::MENU:
                handleMenuInput();
                renderMenu();
                break;

            case State::PLAYING:
                handleGameInput();
                update();
                renderGame();
                break;

            case State::GAME_OVER:
                handleGameOverInput();
                renderGameOver();
                break;
        }

        // Frame rate limiting
        auto elapsed = Clock::now() - frameStart;
        if (elapsed < FRAME_TIME) {
            std::this_thread::sleep_for(FRAME_TIME - elapsed);
        }
    }
}
```

## Input Handling

### Non-blocking Input

```cpp
class ConsoleInput : public InputHandler {
public:
    InputState getState() override {
        InputState state{};
        state.left    = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;
        state.right   = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;
        state.up      = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;
        state.down    = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;
        state.space   = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
        state.enter   = (GetAsyncKeyState(VK_RETURN) & 0x8000) != 0;
        state.escape  = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
        return state;
    }
};
```

### Input Debouncing

To prevent repeated actions when holding keys:

```cpp
class InputHandler {
private:
    InputState previousState_;

public:
    InputEvents getEvents() {
        InputState current = getState();
        InputEvents events{};

        // Only trigger on key press (not hold)
        events.leftPressed    = current.left && !previousState_.left;
        events.rightPressed   = current.right && !previousState_.right;
        events.rotatePressed  = current.up && !previousState_.up;
        events.dropPressed    = current.space && !previousState_.space;

        // Down is special - can be held for continuous soft drop
        events.downHeld       = current.down;

        previousState_ = current;
        return events;
    }
};
```

## Line Clearing Animation

```cpp
int Board::clearLines() {
    int linesCleared = 0;

    // Scan from bottom to top
    for (int y = height_ - 1; y >= 0; --y) {
        if (isLineFull(y)) {
            // Flash effect
            flashLine(y);
            
            // Remove line and shift down
            removeLine(y);
            linesCleared++;
            y++;  // Re-check this position
        }
    }

    return linesCleared;
}

void Board::flashLine(int y) {
    // Brief white flash
    for (int x = 0; x < width_; ++x) {
        grid_[y][x] = Color::WHITE;
    }
    render();
    Sleep(50);
    
    // Then clear
    for (int x = 0; x < width_; ++x) {
        grid_[y][x] = Color::EMPTY;
    }
}
```

## Scoring System

```cpp
class ScoreManager {
public:
    void addLines(int lines) {
        linesCleared_ += lines;
        
        switch (lines) {
            case 1: score_ += 100; break;
            case 2: score_ += 300; break;
            case 3: score_ += 500; break;
            case 4: score_ += 800; break;  // Tetris!
        }
    }

    int getScore() const { return score_; }
    int getLines() const { return linesCleared_; }

private:
    int score_ = 0;
    int linesCleared_ = 0;
};
```

## Class Diagrams

### Tetromino Class

```
┌─────────────────────────────────────────────────────────────┐
│                      Tetromino                              │
├─────────────────────────────────────────────────────────────┤
│ - type_: Type                                               │
│ - rotation_: int (0-3)                                      │
│ - x_: int                                                   │
│ - y_: int                                                   │
├─────────────────────────────────────────────────────────────┤
│ + Tetromino(Type type)                                      │
│ + getCurrentShape(): const std::array<uint8_t, 4>&          │
│ + rotate(): void                                            │
│ + moveLeft(): void                                          │
│ + moveRight(): void                                         │
│ + moveDown(): void                                          │
│ + getX(): int                                               │
│ + getY(): int                                               │
│ + getType(): Type                                           │
│ + getColor(): Color                                         │
└─────────────────────────────────────────────────────────────┘
```

### Board Class

```
┌─────────────────────────────────────────────────────────────┐
│                        Board                                │
├─────────────────────────────────────────────────────────────┤
│ - width_: int                                               │
│ - height_: int                                              │
│ - grid_: std::vector<std::vector<Color>>                    │
├─────────────────────────────────────────────────────────────┤
│ + Board(int width, int height)                              │
│ + canPlace(const Tetromino&): bool                          │
│ + place(const Tetromino&): void                             │
│ + clearLines(): int                                         │
│ + isOccupied(int x, int y): bool                            │
│ + getColor(int x, int y): Color                             │
│ + reset(): void                                             │
├─────────────────────────────────────────────────────────────┤
│ - isLineFull(int y): bool                                   │
│ - removeLine(int y): void                                   │
└─────────────────────────────────────────────────────────────┘
```

### Game Class

```
┌─────────────────────────────────────────────────────────────┐
│                         Game                                │
├─────────────────────────────────────────────────────────────┤
│ - state_: State                                             │
│ - board_: Board                                             │
│ - current_: std::unique_ptr<Tetromino>                      │
│ - next_: std::unique_ptr<Tetromino>                         │
│ - renderer_: std::unique_ptr<Renderer>                      │
│ - input_: std::unique_ptr<InputHandler>                     │
│ - score_: ScoreManager                                      │
│ - dropTimer_: int                                           │
├─────────────────────────────────────────────────────────────┤
│ + Game(unique_ptr<Renderer>, unique_ptr<InputHandler>)      │
│ + run(): void                                               │
├─────────────────────────────────────────────────────────────┤
│ - spawnTetromino(): void                                    │
│ - handleInput(): void                                       │
│ - update(): void                                            │
│ - render(): void                                            │
│ - isGameOver(): bool                                        │
│ - lockPiece(): void                                         │
└─────────────────────────────────────────────────────────────┘
```

## File Organization

### Header Files (include/tetris/)

```
Constants.h     - Game constants (board size, timing, etc.)
Types.h         - Enums (Color, TetrominoType, GameState)
Tetromino.h     - Tetromino class declaration
Board.h         - Board class declaration
Game.h          - Game class declaration
Renderer.h      - Renderer abstract interface
InputHandler.h  - InputHandler abstract interface
Console/
  ConsoleRenderer.h - Windows Console renderer declaration
  ConsoleInput.h    - Windows Console input declaration
```

### Source Files (src/)

```
main.cpp              - Entry point, creates Game and runs
Tetromino.cpp         - Tetromino implementation
Board.cpp             - Board implementation
Game.cpp              - Game logic implementation
Console/
  ConsoleRenderer.cpp - Windows Console rendering
  ConsoleInput.cpp    - Windows Console input handling
```

## Dependencies

- **Windows SDK**: For Console API functions
- **C++ Standard Library**: <vector>, <array>, <memory>, <chrono>
- **CMake 3.16+**: Build system

## Build Configuration

```cmake
cmake_minimum_required(VERSION 3.16)
project(tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tetris
    src/main.cpp
    src/Tetromino.cpp
    src/Board.cpp
    src/Game.cpp
    src/Console/ConsoleRenderer.cpp
    src/Console/ConsoleInput.cpp
)

target_include_directories(tetris PRIVATE include)
```

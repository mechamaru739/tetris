# Implementation Tasks

## Phase 1: Project Setup & Core Data Structures

### Task 1.1: Initialize CMake Project
- [x] Create CMakeLists.txt with C++20 settings
- [x] Set up directory structure (include/, src/)
- [x] Create empty main.cpp
- [x] Verify build compiles (using g++ with MSYS2)

### Task 1.2: Define Constants and Types
- [x] Create `include/tetris/Constants.h`
  - Board dimensions (10x20)
  - Timing constants
  - Scoring values
- [x] Create `include/tetris/Types.h`
  - Color enum (EMPTY, CYAN, YELLOW, PURPLE, BLUE, ORANGE, GREEN, RED)
  - TetrominoType enum (I, O, T, L, J, S, Z)
  - GameState enum (MENU, PLAYING, GAME_OVER)

### Task 1.3: Implement Tetromino Class
- [x] Create `include/tetris/Tetromino.h`
  - Class declaration with all methods
  - Static shape data declaration
- [x] Create `src/Tetromino.cpp`
  - Define all 7 tetromino shapes with 4 rotations each
  - Implement rotation logic
  - Implement movement methods
  - Implement getters

### Task 1.4: Implement Board Class
- [x] Create `include/tetris/Board.h`
  - Class declaration
- [x] Create `src/Board.cpp`
  - Constructor with grid initialization
  - `canPlace()` - collision detection
  - `place()` - lock piece to board
  - `clearLines()` - detect and remove full lines
  - `isOccupied()` and `getColor()` - grid access
  - `reset()` - clear the board

## Phase 2: Rendering System

### Task 2.1: Define Renderer Interface
- [x] Create `include/tetris/Renderer.h`
  - Abstract base class with virtual methods:
    - `clear()`
    - `renderBoard()`
    - `renderTetromino()`
    - `renderNextPiece()`
    - `renderScore()`
    - `renderGameOver()`
    - `renderMenu()`
    - `present()`

### Task 2.2: Implement Console Renderer
- [x] Create `include/tetris/Console/ConsoleRenderer.h`
  - Class declaration inheriting Renderer
  - Private members for console handle and buffer
- [x] Create `src/Console/ConsoleRenderer.cpp`
  - Initialize console handle
  - Implement double buffering with `WriteConsoleOutput()`
  - Implement board rendering with colors
  - Implement tetromino rendering
  - Implement next piece preview box
  - Implement score display
  - Implement menu screen
  - Implement game over screen
  - Color mapping function (Color enum → Windows attributes)

## Phase 3: Input System

### Task 3.1: Define InputHandler Interface
- [x] Create `include/tetris/InputHandler.h`
  - InputState struct (booleans for each key)
  - InputEvents struct (press events)
  - Abstract base class

### Task 3.2: Implement Console Input
- [x] Create `include/tetris/Console/ConsoleInput.h`
  - Class declaration inheriting InputHandler
- [x] Create `src/Console/ConsoleInput.cpp`
  - Implement `getState()` using `GetAsyncKeyState()`
  - Implement key debouncing logic

## Phase 4: Game Logic

### Task 4.1: Implement Game Class
- [x] Create `include/tetris/Game.h`
  - Class declaration with state machine
- [x] Create `src/Game.cpp`
  - Constructor (initialize board, renderer, input)
  - `run()` - main game loop with frame timing
  - `spawnTetromino()` - create new piece, check game over
  - `handleInput()` - process keyboard input
  - `update()` - gravity, line clearing, score
  - `render()` - delegate to renderer
  - `lockPiece()` - place current piece and spawn new one
  - `isGameOver()` - check if new piece can be placed

### Task 4.2: Implement Scoring
- [x] Add score tracking to Game class
- [x] Add line count tracking
- [x] Implement scoring formula (100/300/500/800)
- [x] Display score and lines in render

## Phase 5: Polish & Integration

### Task 5.1: Implement Menu Screen
- [x] Render "TETRIS" title
- [x] Show "Press ENTER to start" prompt
- [x] Handle Enter key to start game
- [x] Handle Escape key to quit

### Task 5.2: Implement Game Over Screen
- [x] Render "GAME OVER" message
- [x] Show final score and lines
- [x] Show "Press ENTER to restart" prompt
- [x] Handle Enter key to restart
- [x] Handle Escape key to quit

### Task 5.3: Add Line Clear Animation
- [x] Flash effect when line is cleared
- [x] Brief pause for visual feedback

### Task 5.4: Edge Cases & Bug Fixes
- [x] Handle wall kicks (rotation near edges)
- [x] Ensure proper piece spawning position
- [x] Test all 7 tetromino rotations
- [x] Verify scoring correctness
- [x] Test rapid input handling

## Phase 6: Testing & Verification

### Task 6.1: Build & Run
- [x] Build with CMake
- [x] Run in Windows Terminal
- [x] Run in cmd.exe
- [x] Verify no crashes

### Task 6.2: Gameplay Testing
- [x] Test all piece types spawn correctly
- [x] Test rotation for all pieces
- [x] Test line clearing (1, 2, 3, 4 lines)
- [x] Test game over condition
- [x] Test restart functionality
- [x] Verify score calculation

### Task 6.3: Visual Polish
- [x] Verify colors display correctly
- [x] Verify no flickering
- [x] Verify layout looks good
- [x] Test different terminal sizes

## Completion Criteria

- [x] All 7 tetromino types work correctly
- [x] Rotation works for all pieces
- [x] Line clearing works (1-4 lines)
- [x] Score updates correctly
- [x] Next piece preview displays
- [x] Menu and game over screens work
- [x] No visual flickering
- [x] Controls are responsive
- [x] Game runs at stable ~60 FPS

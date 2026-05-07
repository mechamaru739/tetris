# Tetris Game - Windows Terminal Edition

## Overview

Build a classic Tetris game that runs in Windows terminal using C++ with modern design patterns.

## Goals

- Classic Tetris gameplay in Windows Console
- Modern C++ architecture (RAII, smart pointers, templates)
- Clean, maintainable codebase with CMake build system

## Scope

### In Scope

- Seven standard Tetromino shapes (I, O, T, L, J, S, Z)
- 10x20 game board
- Score tracking and line clearing
- Next piece preview (1 piece)
- Start menu and game over screen
- Windows Console API rendering with colors
- Unicode box-drawing characters for borders
- Full-block characters (██) for pieces

### Out of Scope

- Ghost piece (shadow showing landing position)
- Sound effects
- Difficulty progression (fixed speed)
- High score persistence
- Multiple next piece preview

## Architecture

### Tech Stack

- **Language**: C++20
- **Build System**: CMake
- **Platform**: Windows (Console API)
- **Design Patterns**: RAII, Smart Pointers, Strategy Pattern

### Project Structure

```
tetris/
├── CMakeLists.txt
├── include/
│   └── tetris/
│       ├── Constants.h
│       ├── Types.h
│       ├── Tetromino.h
│       ├── Board.h
│       ├── Game.h
│       ├── Renderer.h
│       ├── InputHandler.h
│       └── Console/
│           ├── ConsoleRenderer.h
│           └── ConsoleInput.h
└── src/
    ├── main.cpp
    ├── Tetromino.cpp
    ├── Board.cpp
    ├── Game.cpp
    └── Console/
        ├── ConsoleRenderer.cpp
        └── ConsoleInput.cpp
```

### Core Classes

1. **Tetromino** - Represents a tetris piece
   - Stores 4 rotation states using bit masks
   - Position (x, y) on the board
   - Type and color information

2. **Board** - Manages the game grid
   - 10x20 grid storing color values
   - Collision detection
   - Line clearing logic

3. **Game** - Main game logic
   - State machine (MENU, PLAYING, GAME_OVER)
   - Game loop with input/update/render cycle
   - Score and line count tracking

4. **Renderer** (interface) - Abstract rendering
   - ConsoleRenderer implementation using Windows Console API
   - Double-buffered rendering to prevent flicker

5. **InputHandler** (interface) - Abstract input
   - ConsoleInput implementation using GetAsyncKeyState

## Game Design

### Tetromino Shapes

```
I: ████      O: ██      T: ███      L: █
               ██          █          █
                                         ██

J:  █       S:  ██     Z: ██
    █           ██         ██
   ██
```

### Controls

- **Left/Right Arrow**: Move piece horizontally
- **Up Arrow**: Rotate piece clockwise
- **Down Arrow**: Soft drop (faster fall)
- **Space**: Hard drop (instant drop)
- **Enter**: Start game / Restart
- **Escape**: Quit

### Screen Layout

```
┌──────────────────────┐  ┌─────────────┐
│                      │  │  NEXT:      │
│                      │  │  ┌─────┐    │
│                      │  │  │ ██  │    │
│                      │  │  │  █  │    │
│                      │  │  └─────┘    │
│     GAME BOARD       │  │             │
│      (10x20)         │  │  SCORE:     │
│                      │  │  12500      │
│                      │  │             │
│                      │  │  LINES:     │
│                      │  │  42         │
│                      │  │             │
└──────────────────────┘  └─────────────┘
```

### Scoring

- 1 line: 100 points
- 2 lines: 300 points
- 3 lines: 500 points
- 4 lines (Tetris): 800 points

## Implementation Plan

### Phase 1: Core Foundation
- [ ] Set up CMake project structure
- [ ] Implement Constants and Types
- [ ] Implement Tetromino class with rotation
- [ ] Implement Board class with collision detection

### Phase 2: Game Logic
- [ ] Implement Game class with state machine
- [ ] Implement line clearing and scoring
- [ ] Implement piece spawning and game over detection

### Phase 3: Rendering
- [ ] Implement ConsoleRenderer with double buffering
- [ ] Render game board with colors
- [ ] Render next piece preview
- [ ] Render score and lines

### Phase 4: Input & Polish
- [ ] Implement ConsoleInput with non-blocking input
- [ ] Add start menu screen
- [ ] Add game over screen
- [ ] Polish and bug fixes

## Risks & Mitigations

| Risk | Impact | Mitigation |
|------|--------|------------|
| Console flickering | High | Use double buffering with WriteConsoleOutput |
| Input latency | Medium | Use GetAsyncKeyState for non-blocking input |
| Unicode rendering issues | Low | Test on Windows Terminal and cmd.exe |

## Success Criteria

- Game runs smoothly at 60 FPS
- All 7 tetromino types work correctly
- Line clearing and scoring work properly
- No visual flickering
- Responsive controls

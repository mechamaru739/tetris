#include "tetris/Console/ConsoleInput.h"
#include <windows.h>

namespace tetris {

InputState ConsoleInput::getState() {
    InputState state{};
    state.left = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;
    state.right = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;
    state.up = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;
    state.down = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;
    state.space = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
    state.enter = (GetAsyncKeyState(VK_RETURN) & 0x8000) != 0;
    state.escape = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
    return state;
}

} // namespace tetris

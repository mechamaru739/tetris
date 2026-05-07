#pragma once

#include "tetris/InputHandler.h"

namespace tetris {

class ConsoleInput : public InputHandler {
public:
    InputState getState() override;
};

} // namespace tetris

#include "tetris/Game.h"
#include "tetris/Console/ConsoleRenderer.h"
#include "tetris/Console/ConsoleInput.h"
#include <memory>

int main() {
    auto renderer = std::make_unique<tetris::ConsoleRenderer>();
    auto input = std::make_unique<tetris::ConsoleInput>();

    tetris::Game game(std::move(renderer), std::move(input));
    game.run();

    return 0;
}

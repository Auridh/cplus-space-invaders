#include "model_simulator_game.h"
#include "view_console.h"
#include "controller_console.h"

int main() {
    auto* game = new GameModel();
    auto* view = new ConsoleView(game);
    auto* controller = new ConsoleController(game, view);

    wchar_t ch = '\0';
    while (ch != 'q') {
        ch = controller->getInput();
        game->simulate_game_step();
        napms(25);
    }

    delete controller;
    delete game;
    delete view;
    return 0;
}
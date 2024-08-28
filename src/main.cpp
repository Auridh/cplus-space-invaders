#include <cstddef>
#include <cstdlib>
#include <ctime>

#include "model_simulator_game.h"
#include "view_console.h"
#include "controller_console.h"

int main() {
    // update random seed
    // so that randomised events are different each game session
    srand(time(nullptr));

    // create the main game objects
    auto* game = new GameModel();
    auto* view = new ConsoleView(game);
    auto* controller = new ConsoleController(game, view);

    // the game loops until "q" is pressed
    wchar_t ch = '\0';
    while (ch != 'q') {
        ch = controller->getInput();
        game->simulate_game_step();
    }

    // free memory
    delete controller;
    delete game;
    delete view;
}
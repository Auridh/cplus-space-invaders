#include <cstddef>
#include <cstdlib>
#include <ctime>

#include "view_console.h"
#include "controller_console.h"

int main() {
    // update random seed
    // so that randomised events are different each game session
    srand(time(nullptr));

    // create the main game objects
    auto* game = new GameModel();
    auto* view = new ConsoleView(game);
    Controller* controller = new ConsoleController(game, view);
    Controller* switchController = new AutomatedController(game, view);

    // the game loops until "q" is pressed
    wchar_t ch = '\0';
    while (ch != 'q') {
        ch = controller->getInput();

        // switch controller if necessary
        if (ch == 's') {
            const auto temp = controller;
            controller = switchController;
            switchController = temp;
        }

        game->simulate_game_tick();
    }

    // free memory
    delete controller;
    delete game;
    delete view;
}
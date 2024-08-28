#include "controller_console.h"
#include "game_model.h"

// constructor assigning initial property values
ConsoleController::ConsoleController(GameModel* model, ConsoleView* view) {
    this->view = view;
	this->model = model;
};

// override the getInput() function to get input from the console
wchar_t ConsoleController::getInput() {
    // get the input
    auto ch = wgetch(view->getWindow());
    // if the input is valid
    if (ch != ERR) {
        // process it
        model->control_player(ch);
    }
    // return the input
    // (required for quitting the game gracefully and switching controllers)
    return ch;
}


// constructor assigning initial property values
AutomatedController::AutomatedController(GameModel* model, ConsoleView* view) {
    this->view = view;
    this->model = model;
};

// override the getInput() function to get input from the console
wchar_t AutomatedController::getInput() {
    // TODO: auto play the game


    // return the input
    // (required for quitting the game gracefully and switching controllers)
    return wgetch(view->getWindow());
};

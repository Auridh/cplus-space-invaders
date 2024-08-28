#include "controller_console.h"
#include "game_model.h"

ConsoleController::ConsoleController(GameModel* model, ConsoleView* view) {
    this->view = view;
	this->model = model;
};

wchar_t ConsoleController::getInput() {
    auto ch = wgetch(view->getWindow());
    if (ch != ERR) {
        model->control_player(ch);
    }
    return ch;
};

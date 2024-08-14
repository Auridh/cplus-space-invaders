#include "controller_console.h"

ConsoleController::ConsoleController(GameModel* model) {
	this->model = model;
};

wchar_t ConsoleController::getInput() {
    wchar_t ch = getch();
    model->control_player(ch);
    return ch;
    return ch;
};

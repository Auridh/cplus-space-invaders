#ifndef CONTROLLER_GAME_H_ // header guard to prevent multiple includes
#define CONTROLLER_GAME_H_

#include "game_model.h"
#include "view_console.h"

class Controller
{
public:
    virtual ~Controller() = default;
    // You should consider how to keep this as general as possible. And should set the type accordingly.
    virtual wchar_t getInput() = 0; // declare a pure virtual function for getting user input
};

class ConsoleController : public Controller // derive ConsoleController class from the Controller base class
{
    GameModel* model; // pointer to the GameModel object
    ConsoleView* view;

public:
    ConsoleController(GameModel* model, ConsoleView* view); // constructor that takes a GameModel pointer as parameter

    wchar_t getInput() override; // override the getInput() function to get input from the console
};

#endif

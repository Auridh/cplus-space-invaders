// header guard to prevent multiple includes
#ifndef CONTROLLER_GAME_H_
#define CONTROLLER_GAME_H_

#include "game_model.h"
#include "view_console.h"

class Controller
{
public:
    virtual ~Controller() = default;
    // You should consider how to keep this as general as possible. And should set the type accordingly.
    // declare a pure virtual function for getting user input
    virtual wchar_t getInput() = 0;
};

// derive ConsoleController class from the Controller base class
class ConsoleController : public Controller
{
    // pointer to the GameModel object
    GameModel* model;
    ConsoleView* view;

public:
    // constructor that takes a GameModel pointer as parameter
    ConsoleController(GameModel* model, ConsoleView* view);
    // override the getInput() function to get input from the console
    wchar_t getInput() override;
};

// derive ConsoleController class from the Controller base class
class AutomatedController : public Controller
{
    // pointer to the GameModel object
    GameModel* model;
    ConsoleView* view;

public:
    // constructor that takes a GameModel pointer as parameter
    AutomatedController(GameModel* model, ConsoleView* view);
    // override the getInput() function to get input from the console
    wchar_t getInput() override;
};

#endif

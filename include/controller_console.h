#ifndef CONTROLLER_GAME_H_ // header guard to prevent multiple includes
#define CONTROLLER_GAME_H_

#include "model_simulator_game.h" // include the Game Model header file
#include <ncurses.h> // include the ncurses library for console input/output

class Controller
{
public:
    // You should consider how to keep this as general as possible. And should set the type accordingly.
    virtual wchar_t getInput() = 0; // declare a pure virtual function for getting user input
};

class ConsoleController : public Controller // derive ConsoleController class from the Controller base class
{
    GameModel* model; // pointer to the GameModel object

public:
    ConsoleController(GameModel* model); // constructor that takes a GameModel pointer as parameter

    wchar_t getInput(); // override the getInput() function to get input from the console
};

#endif  // end of header guard

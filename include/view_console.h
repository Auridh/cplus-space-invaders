// #pragma once directive added to avoid multiple inclusions of header files
#pragma once

#include <ncurses.h>

#include "game_model.h"
#include "observer.h"
#include "model_simulator_game.h"

// Inheriting from Observer class
// Class for displaying the game in the console
class ConsoleView : public Observer
{
    // Pointer variable of GameModel class
    GameModel* model;
    // Game window
    WINDOW* window;

    // initialize the window, colors and borders
    void setupView();
    // draw the game to the console
    void draw();

    // draw functions for single game objects of vectors of game objects
    void drawObject(Drawable* drawable);
    template<class T> void drawObjects(const std::vector<T*>& drawables)
        requires(std::is_base_of_v<Drawable, T>);
    template<class T> void drawObjects(std::vector<std::vector<T*>> drawables)
        requires(std::is_base_of_v<Drawable, T>);

public:
    // constructor taking the game model as an argument
    explicit ConsoleView(GameModel* model);
    // destructor
    ~ConsoleView() override;

    // update function called each game loop
    void update() override;

    // get the (ncurses) window the game is drawn in
    WINDOW* getWindow();
};

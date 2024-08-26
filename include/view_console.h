// #pragma once directive added to avoid multiple inclusions of header files
#pragma once

#include <ncurses.h>
#include "observer.h"
#include "model_simulator_game.h"

// Inheriting from Observer class
class ConsoleView : public Observer
{
    // Pointer variable of GameModel class
    GameModel* model;

    // Wall texture character
    char wallTexture = 'X';
    // Window
    WINDOW* window;

    void setupView();
    void draw();

    void drawObject(Drawable* drawable);
    template<class T> void drawObjects(const std::vector<T*>& drawables)
        requires(std::is_base_of_v<Drawable, T>);
    template<class T> void drawObjects(std::vector<std::vector<T*>> drawables)
        requires(std::is_base_of_v<Drawable, T>);

public:
    explicit ConsoleView(GameModel* model);
    ~ConsoleView() override;

    void update() override;

    WINDOW* getWindow();
};

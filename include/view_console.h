#pragma once  // #pragma once directive added to avoid multiple inclusions of header files

#include "observer.h"  // Include Observer header file
#include "model_simulator_game.h"  // Include GameModel header file
#include <ncurses.h>

class ConsoleView : public Observer  // Inheriting from Observer class
{
    GameModel* model;  // Pointer variable of GameModel class

public:
    ConsoleView(GameModel* model);  // Constructor of ConsoleView class

    virtual ~ConsoleView();  // Virtual destructor of ConsoleView class

    void update();  // Function to update the screen

    void drawPlayer(int y, int x);  // Function to draw the player

    void drawAlien(int y, int x);   // Function to draw an alien

    void drawAliens();   // Function to draw all aliens

    void drawProjectiles();

    void drawProjectile(Projectile* p);

private:
    // Textures
    char wallTexture = 'X';  // Wall texture character

    void setup_view();  // Function to set up the view
};

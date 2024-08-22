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

    void drawPlayer(Player* player);
    void drawAlien(Alien* alien);
    void drawAliens();
    void drawProjectiles();
    void drawProjectile(Projectile* p);
    void drawExplosion(Explosion* e);
    void drawExplosions();

public:
    ConsoleView(GameModel* model);
    virtual ~ConsoleView();

    void update();

    WINDOW* getWindow();
};

#include "view_console.h"
#include <ncurses.h>
#include <stdlib.h>


ConsoleView::ConsoleView(GameModel* model) {
	setup_view();
	this->model = model;
	this->model->addObserver(this);
};

ConsoleView::~ConsoleView() {
    endwin();
};

void ConsoleView::update() {
    // libncurses standard loop calls
    erase();
    refresh();

    // Example for building the game view
    for(int i = 0; i < model->getGameWidth(); i++) {
        mvaddch(3, i, wallTexture);
    }
    for(int i = 3; i < model->getGameHeight(); i++) {
        mvaddch(i, 0, wallTexture);
        mvaddch(i, model->getGameWidth() - 1, wallTexture);
    }

    // Show points of player
    mvprintw(1, model->getGameWidth() / 2 / 2, "Score: %i  Level: %i", model->getScore(), model->getLevel());

    // Draw different objects. 
    drawPlayer(model->getPlayer().getY(), model->getPlayer().getX());
    drawAliens();
    drawProjectiles();
    drawExplosions();
};

void ConsoleView::setup_view() {
    // Init ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(30);
};

void ConsoleView::drawPlayer(int y, int x) {
    mvaddch(y-1, x, 'A');
};

void ConsoleView::drawAlien(Alien* a) {
    mvaddch(a->getY()-1, a->getX(), 'Y');
};

void ConsoleView::drawProjectile(Projectile* p) {
    mvaddch(p->getY()-1, p->getX(), '.');
}


void ConsoleView::drawAliens() {
    for (auto & alienRow : *model->getAliens()) {
        for (auto & alien : alienRow) {
            drawAlien(alien);
        }
    }
};

void ConsoleView::drawProjectiles() {
    for (auto & projectile : model->getProjectiles()) {
        drawProjectile(projectile);
    }
}

void ConsoleView::drawExplosion(Explosion* e) {
    switch (e->getExplosionState())
    {
        case 1:
            mvaddch(e->getY()-1, e->getX(), '*');
            break;
        case 2:
            mvaddch(e->getY()-1, e->getX(), '#');
            break;
        case 3:
            mvaddch(e->getY()-1, e->getX(), '@');
            break;
        default:
            break;
    }
}

void ConsoleView::drawExplosions() {
    for (auto & explosion : model->getExplosions()) {
        drawExplosion(explosion);
    }
}




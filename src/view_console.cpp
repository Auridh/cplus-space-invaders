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

void ConsoleView::drawAlien(int y, int x) {
    mvaddch(y-1, x, 'Y');
};

void ConsoleView::drawAliens() {
    Alien*** aliens = model->getAliens();
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 9; j++) {
            if (aliens[i][j])
                drawAlien(aliens[i][j]->getY(), aliens[i][j]->getX());
        }
    }
};
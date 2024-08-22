#include "view_console.h"
#include <ncurses.h>

ConsoleView::ConsoleView(GameModel *model) {
    this->model = model;
    this->model->addObserver(this);
    setupView();
};

ConsoleView::~ConsoleView() {
    delwin(window);
    endwin();
};

void ConsoleView::update() {
    // libncurses standard loop calls
    werase(window);
    box(window, 0, 0);
    wmove(window, 3, 1);
    whline(window, ACS_HLINE, model->getGameWidth()-2);
    draw();
}

WINDOW * ConsoleView::getWindow() {
    return window;
};

void ConsoleView::setupView() {
    // Init ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    window = newwin(model->getGameHeight(), model->getGameWidth(), 0, 0);
    nodelay(window, TRUE);
    keypad(window, TRUE);

    timeout(30);
}

void ConsoleView::draw() {
    // Show points of player
    mvwprintw(window, 1, model->getGameWidth() / 2 / 2, "Score: %i  Level: %i", model->getScore(), model->getLevel());

    // Draw different objects.
    drawPlayer(model->getPlayer());
    drawAliens();
    drawProjectiles();
    drawExplosions();
};

void ConsoleView::drawPlayer(Player* p) {
    mvwaddch(window, p->getY()-1, p->getX(), 'A');
};

void ConsoleView::drawAlien(Alien *a) {
    mvwaddch(window, a->getY()-1, a->getX(), 'Y');
};

void ConsoleView::drawProjectile(Projectile *p) {
    mvwaddch(window, p->getY()-1, p->getX(), '.');
}


void ConsoleView::drawAliens() {
    for (auto &alienRow: *model->getAliens()) {
        for (auto &alien: alienRow) {
            drawAlien(alien);
        }
    }
};

void ConsoleView::drawProjectiles() {
    for (auto &projectile: model->getProjectiles()) {
        drawProjectile(projectile);
    }
}

void ConsoleView::drawExplosion(Explosion *e) {
    switch (e->getExplosionState()) {
        case 1:
            mvwaddch(window, e->getY()-1, e->getX(), '*');
            break;
        case 2:
            mvwaddch(window, e->getY()-1, e->getX(), '#');
            break;
        case 3:
            mvwaddch(window, e->getY()-1, e->getX(), '@');
            break;
        default:
            break;
    }
}

void ConsoleView::drawExplosions() {
    for (auto &explosion: model->getExplosions()) {
        drawExplosion(explosion);
    }
}

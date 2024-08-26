#include "view_console.h"
#include <ncurses.h>
#include <string>

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

    if (has_colors()) {
        use_default_colors();
        start_color();
        init_pair(1, COLOR_WHITE, -1);
        init_pair(2, COLOR_BLUE, -1);
        init_pair(3, COLOR_RED, -1);
        init_pair(4, COLOR_YELLOW, -1);
        init_pair(5, COLOR_GREEN, -1);
    }

    timeout(30);
}

void ConsoleView::draw() {
    // Show points of player
    auto scoreText = "Score: " + std::to_string(model->getScore()) + "  Level: " + std::to_string(model->getLevel());
    mvwprintw(window, 1, model->getGameWidth() / 2 - scoreText.length() / 2, scoreText.c_str());
    if(model->getTimeout() > model->LEVEL_TIMEOUT + 1 || model->getTimeout() == 0)
        mvwprintw(window, 2, model->getGameWidth() / 2 - 4, "Health: %i", model->getPlayer()->getHealth());
    if (model->getTimeout() > 0 && model->getTimeout() < model->LEVEL_TIMEOUT + 1)
        mvwprintw(
            window,
            (model->getGameHeight() + 2) / 2,
            model->getGameWidth() / 2 - 7,
            "Next Level in %i",
            model->getTimeout() / 40 + 1);
    else if (model->getTimeout() > model->LEVEL_TIMEOUT)
        mvwprintw(
            window,
            (model->getGameHeight() + 2) / 2,
            model->getGameWidth() / 2 - 4,
            "Game Over");

    // Draw different objects.
    drawObject(model->getPowerUp());
    drawObject(model->getPlayer());
    drawObjects(model->getAliveAliens());
    drawObjects(model->getProjectiles());
    drawObjects(model->getExplosions());
};

void ConsoleView::drawObject(Drawable* drawable) {
    if(!drawable)
        return;

    wattron(
        window,
        COLOR_PAIR(drawable->getColor()));
    mvwaddch(
        window,
        drawable->getY()-1,
        drawable->getX(),
        drawable->getTexture());
    wattroff(
        window,
        COLOR_PAIR(drawable->getColor()));
}

template<class T>
void ConsoleView::drawObjects(const std::vector<T*>& drawables)
requires (std::is_base_of_v<Drawable, T>)
{
    for (auto &drawable: drawables) {
        drawObject(drawable);
    }
}

template<class T>
void ConsoleView::drawObjects(std::vector<std::vector<T*>> drawables)
requires (std::is_base_of_v<Drawable, T>)
{
    for (auto &row: drawables) {
        drawObjects(row);
    }
}

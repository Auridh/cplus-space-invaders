#include "controller_console.h"

#include <valarray>

#include "game_model.h"

// constructor assigning initial property values
ConsoleController::ConsoleController(GameModel* model, ConsoleView* view) {
    this->view = view;
	this->model = model;
};

// override the getInput() function to get input from the console
wchar_t ConsoleController::getInput() {
    // get the input
    auto ch = wgetch(view->getWindow());
    // if the input is valid
    if (ch != ERR) {
        // process it
        auto player = model->getPlayer();
        if ((ch == 'a' || ch==KEY_LEFT) && player->getX() > 1)
            player->setX(player->getX() - 1);
        if ((ch == 'd' || ch==KEY_RIGHT) && player->getX() < model->getGameWidth() - 2)
            player->setX(player->getX() + 1);
        if (ch == ' ')
            model->playerShoot();
    }
    // return the input
    // (required for quitting the game gracefully and switching controllers)
    return ch;
}


// constructor assigning initial property values
AutomatedController::AutomatedController(GameModel* model, ConsoleView* view) {
    this->view = view;
    this->model = model;
};

// override the getInput() function to get input from the console
wchar_t AutomatedController::getInput() {
    auto player = model->getPlayer();

    // shoot incoming projectiles
    bool shoot = false;
    for (auto & projectile : model->getProjectiles()) {
        if (projectile->getVelocity() < 0 || projectile->getX() != player->getX())
            continue;

        shoot = true;
        break;
    }

    // if a powerup is available get it first
    int playerOffset = model->getGameWidth();
    if (model->getPowerUp()) {
        playerOffset = model->getPowerUp()->getX() - player->getX();
    } else {
        // else look for the nearest alien
        auto aliens = model->getAliveAliens();
        for (auto & alienRow : aliens) {
            for (short i = alienRow.size() - 1; i >= 0; i--) {
                short offset = alienRow[i]->getX() - player->getX();
                if (std::abs(offset) < std::abs(playerOffset))
                    playerOffset = offset;
                if (alienRow[i]->getX() == player->getX())
                    shoot = true;
            }
        }
    }

    // prioritize shooting at incoming projectiles
    if (shoot)
        model->playerShoot();
    // else move the player in the determined direction
    else if (playerOffset != 0 && playerOffset != model->getGameWidth())
        player->setX(player->getX() + (playerOffset > 0 ? 1 : -1));

    // return the input
    // (required for quitting the game gracefully and switching controllers)
    return wgetch(view->getWindow());
};

// Wie man eine alternative Control-Möglichkeit implementieren könnte:
//
// Erstelle dafür zuerst eine neue Klasse, die von Controller erbt.
// Im Falle von einem Trackpad könnten wir diese sinnvollerweise TrackpadController nennen.
// Nun übernehmen wir den Konstruktor vom Controller und implementieren die getInput() Methode neu.
// Dafür schauen wir nach, wie wir die Trackpad-Inputs erhalten und rufen dann abhängig von diesen
// entweder model->playerShoot() auf oder wir ändern die Position von player bzgl. x um diesen nach
// links oder nach rechts zu bewegen.
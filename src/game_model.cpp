#include "game_model.h"

#include <cstdlib>
#include <ncurses.h>

// Game model implementation
// constructor with initial values and alien generation
GameModel::GameModel() : player(width/2, height-1), powerUp(nullptr)
{
    createAliens();
}
// Destructor freeing memory
GameModel::~GameModel() {
    // delete aliens
    for (auto & row : aliens) {
        for (auto & alien : row) {
            delete alien;
        }
    }
    // delete projectiles
    for (auto & projectile : projectiles) {
        delete projectile;
    }
    // delete explosions
    for (auto & explosion : explosions) {
        delete explosion;
    }
    // delete the PowerUp
    delete powerUp;
};

// standard getters
int GameModel::getGameWidth() {
    return width;
};
int GameModel::getGameHeight() {
    return height;
};
int GameModel::getLevel() {
    return level;
};
int GameModel::getScore() {
    return score;
};
Player* GameModel::getPlayer() {
    return &player;
}

PowerUp * GameModel::getPowerUp() {
    return powerUp;
};

// the getAliveAliens methods filters all aliens by their isDead value
std::vector<std::vector<Alien*>> GameModel::getAliveAliens() {
    std::vector<std::vector<Alien*>> alive = {};

    for (auto & alienRow : aliens) {
        std::vector<Alien*> newRow = {};
        for (auto & alien : alienRow) {
            if (!alien->isDead())
                newRow.push_back(alien);
        }
        alive.push_back(newRow);
    }

    return alive;
};

std::vector<Projectile*> GameModel::getProjectiles() {
    return projectiles;
};
std::vector<Explosion *> GameModel::getExplosions() {
    return explosions;
}

int GameModel::getTimeout() {
    return timeoutTicks;
}

// creates aliens in six rows
// eight per even and nine per uneven row
void GameModel::createAliens() {
    for(int i = 0; i < 6; i++) {
        std::vector<Alien*> row = {};
        bool uneven = i % 2 == 0;

        for(int j = 0; j < (uneven ? 9 : 8); j++) {
            auto alien = new Alien();
            row.push_back(alien);
        }

        aliens.push_back(row);
    }
};
// aliens are created "dead" but we need them alive to shoot them...
void GameModel::spawnAliens() {
    for (int i = 0; i < aliens.size(); i++) {
        bool uneven = i % 2 == 0;
        for(int j = 0; j < (uneven ? 9 : 8); j++)
            aliens[i][j]->revivify(j * 4 + (uneven ? 2 : 4), i * 2 + 6);
    }
}
// adds a projectile to the game
void GameModel::addProjectile(Projectile* projectile)
{
    projectiles.push_back(projectile);
};
// adds a explosion to the game
void GameModel::addExplosion(Explosion *explosion) {
    explosions.push_back(explosion);
};

// increase the game score
void GameModel::increaseScore(int value) {
    score += value;
}
// increase the level
void GameModel::increaseLevel() {
    level++;
}

void GameModel::control_player(wchar_t ch)
{
    if (ch==KEY_LEFT && player.getX() > 1)
    {
        player.setX(player.getX() - 1);
    }
    if (ch==KEY_RIGHT && player.getX() < width - 2)
    {
        player.setX(player.getX() + 1);
    }
    if (ch==' ' && timeoutTicks == 0)
    {
        addProjectile(
            new Projectile(
                player.getX(),
                player.getY()-1,
                -5,
                COLOR_WHITE));

        // If PowerUp activated
        if(player.getPowerUpTime() > 0) {

            addProjectile(
                new Projectile(
                    player.getX()-1,
                    player.getY()-1,
                    -5,
                    COLOR_WHITE));

            addProjectile(
                new Projectile(
                    player.getX()+1,
                    player.getY()-1,
                    -5,
                    COLOR_WHITE));
        }
    }
};

void GameModel::simulate_game_step()
{
    napms(TIME_PER_TICK);

    if(player.getHealth() <= 0 && timeoutTicks == 0) {
        gameOver();
    }

    ticks++;

    // Implement game dynamics.
    notifyUpdate();

    updateProjectiles();
    checkCollisions();
    updateExplosions();
    spawnPowerUp();
    player.decreasePowerUpTime();

    if (timeoutTicks > 0) {
        timeoutTicks--;
        if (timeoutTicks == 0) {
            spawnAliens();
            ticks = 0;

            if (player.getHealth() <= 0)
                player.resetHealth();
        }
        return;
    }

    updateAliens();

    if (ticks / TICKS_PER_ALIEN_CHANGE == 8) {
        ticks = 0;
    }
}

// private game model functions
void GameModel::updateAliens()
{
    int step = ticks / TICKS_PER_ALIEN_CHANGE;
    for (int i = 0; i < aliens.size(); i++) {
        for (int j = 0; j < aliens[i].size(); j++) {
            if (aliens[i][j]->isDead())
                continue;

            // projectiles
            if (i >= aliens.size() - 2
                || (aliens[i+2][j]->isDead()
                    && (i+4 >= aliens.size()
                    || aliens[i+4][j]->isDead()))) {
                bool shoot = rand() % 1001 < 2 * level;
                if (shoot)
                    addProjectile(new Projectile(
                        aliens[i][j]->getX(),
                        aliens[i][j]->getY()+1,
                        1+level,
                        COLOR_GREEN));
            }

            // movement every TICKS_PER_ALIEN_CHANGE ticks
            if (ticks % TICKS_PER_ALIEN_CHANGE == 0) {
                if (step == 0 || step == 4 || step == 8) {
                    aliens[i][j]->setY(aliens[i][j]->getY() + 1);
                    if(aliens[i][j]->getY() >= height - 1) {
                        gameOver();
                        return;
                    }
                    continue;
                }
                if (step <= 3) {
                    aliens[i][j]->setX(aliens[i][j]->getX() + 1);
                    continue;
                }
                if (step <= 7) {
                    aliens[i][j]->setX(aliens[i][j]->getX() - 1);
                }
            }
        }
    }
}

void GameModel::updateProjectiles()
{
    std::vector<Projectile*> toRemove = {};

    for (auto & projectile : projectiles) {
        if (ticks % projectile->getVelocity() == 0) {
            continue;
        }
        if (projectile->getY() <= 5 || projectile->getY() >= height - 1) {
            toRemove.push_back(projectile);
            continue;
        }
        projectile->setY(projectile->getY() + (projectile->getVelocity() > 0 ? 1 : -1));
    }
    for (auto & projectile : toRemove) {
        hitProjectile(projectile);
    }
}

void GameModel::updateExplosions() {
    std::vector<Explosion*> toRemove = {};

    for (auto & explosion : explosions) {
        explosion->increaseExplosionState();
        if(explosion->getExplosionState() == 4) {
            toRemove.push_back(explosion);
        }
    }
    for (auto & explosion : toRemove) {
        deleteExplosion(explosion);
    }
}

void GameModel::hitAlien(Alien *alien) {
    score += 1 * level;
    addExplosion(new Explosion(alien->getX(), alien->getY()));
    deleteAlien(alien);
    checkNextLevel();
}

void GameModel::hitProjectile(Projectile *projectile) {
    addExplosion(new Explosion(projectile->getX(), projectile->getY()));
    deleteProjectile(projectile);
}

void GameModel::deleteAlien(Alien *alien) {
    if(alien->isDead()) return;
    alien->kill();
}

void GameModel::deleteProjectile(Projectile *projectile) {
    if(!projectile) return;
    std::erase(projectiles, projectile);
    delete projectile;
}

void GameModel::deleteExplosion(Explosion *explosion) {
    if(!explosion) return;
    std::erase(explosions, explosion);
    delete explosion;
}

void GameModel::spawnPowerUp() {
    bool spawn = rand() % 1001 < 2;
    if (spawn) {
        int x = rand() % (width - 2) + 1;
        if(powerUp) {
            powerUp->setX(x);
            powerUp->setType(x % 2);
            return;
        }
        powerUp = new PowerUp(x, player.getY());
        powerUp->setType(x % 2);
    }
}

void GameModel::checkCollisions() {
    // Check collisions Player with PowerUp
    if(powerUp && player.getX() == powerUp->getX()) {
        player.takePowerUp(powerUp->getType());
        delete powerUp;
        powerUp = nullptr;
    }

    // Check collisions projectiles
    std::vector<Projectile*> toRemoveProjectiles = {};
    std::vector<Projectile*> toRemoveProjectilesAlienProjectileCase = {};

    for (auto & projectile : projectiles) {
        // Projektile vom Spieler
        if (projectile->getVelocity() < 0) {
            // Trifft es ein Alien?
            for (auto & alienRow : aliens) {
                for (auto & alien : alienRow) {
                    if(!alien->isDead()
                        && alien->getX() == projectile->getX()
                        && alien->getY() == projectile->getY()) {
                        toRemoveProjectiles.push_back(projectile);
                        hitAlien(alien);
                    }
                }
            }
            // Trifft es ein Projektil von einem Alien?
            for (auto & projectileAlien : projectiles) {
                if(projectileAlien->getVelocity() > 0
                            && (projectileAlien->getY() == projectile->getY() || projectileAlien->getY() + 1 == projectile->getY())
                            && projectileAlien->getX() == projectile->getX()) {
                    toRemoveProjectilesAlienProjectileCase.push_back(projectile);
                    toRemoveProjectilesAlienProjectileCase.push_back(projectileAlien);
                }
            }
        } else {
            // Projektile vom Alien
            if(projectile->getY() == player.getY()
                            && projectile->getX() == player.getX()) {
                toRemoveProjectilesAlienProjectileCase.push_back(projectile);
                player.decreaseHealth();
            }
        }
    }
    for (auto & projectile : toRemoveProjectiles) {
        deleteProjectile(projectile);
    }
    for (auto & projectile : toRemoveProjectilesAlienProjectileCase) {
        hitProjectile(projectile);
    }
}

void GameModel::checkNextLevel() {
    auto alive = getAliveAliens();
    for (auto & row : alive)
        if (!row.empty())
            return;

    startNextLevel();
}

void GameModel::startNextLevel() {
    level++;
    ticks = INITIAL_TICKS;
    timeoutTicks = LEVEL_TIMEOUT;
}

void GameModel::gameOver() {
    player.kill();

    for (auto & alienRow : aliens) {
        for (auto & alien : alienRow) {
            deleteAlien(alien);
        }
    }

    score = INITIAL_SCORE;
    level = INITIAL_LEVEL;
    ticks = INITIAL_TICKS;
    timeoutTicks = GAME_OVER_TIMEOUT;
}

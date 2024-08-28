#include "game_model.h"

#include <cstdlib>
#include <ncurses.h>

// Game model implementation
// constructor with initial values and alien generation
game_model::game_model() : player(width/2, height-1), powerUp(nullptr)
{
    createAliens();
}
// Destructor freeing memory
game_model::~game_model() {
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
int game_model::getGameWidth() {
    return width;
};
int game_model::getGameHeight() {
    return height;
};
int game_model::getLevel() {
    return level;
};
int game_model::getScore() {
    return score;
};
Player* game_model::getPlayer() {
    return &player;
}

PowerUp * game_model::getPowerUp() {
    return powerUp;
};

// the getAliveAliens methods filters all aliens by their isDead value
std::vector<std::vector<Alien*>> game_model::getAliveAliens() {
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

std::vector<Projectile*> game_model::getProjectiles() {
    return projectiles;
};
std::vector<Explosion *> game_model::getExplosions() {
    return explosions;
}

int game_model::getTimeout() {
    return timeoutTicks;
}

// creates aliens in six rows
// eight per even and nine per uneven row
void game_model::createAliens() {
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
void game_model::spawnAliens() {
    for (int i = 0; i < aliens.size(); i++) {
        bool uneven = i % 2 == 0;
        for(int j = 0; j < (uneven ? 9 : 8); j++)
            aliens[i][j]->revivify(j * 4 + (uneven ? 2 : 4), i * 2 + 6);
    }
}
// adds a projectile to the game
void game_model::addProjectile(Projectile* projectile)
{
    projectiles.push_back(projectile);
};
// adds a explosion to the game
void game_model::addExplosion(Explosion *explosion) {
    explosions.push_back(explosion);
};

// increase the game score
void game_model::increaseScore(int value) {
    score += value;
}
// increase the level
void game_model::increaseLevel() {
    level++;
}

void game_model::control_player(wchar_t ch)
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
                1));

        // If PowerUp activated
        if(player.getPowerUpTime() > 0) {

            addProjectile(
            new Projectile(
                player.getX()-1,
                player.getY()-1,
                -5,
                1));

            addProjectile(
            new Projectile(
                player.getX()+1,
                player.getY()-1,
                -5,
                1));
        }
    }
};

void game_model::simulate_game_step()
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
void game_model::updateAliens()
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
                        5));
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

void game_model::updateProjectiles()
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

void game_model::updateExplosions() {
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

void game_model::hitAlien(Alien *alien) {
    score += 1 * level;
    addExplosion(new Explosion(alien->getX(), alien->getY()));
    deleteAlien(alien);
    checkNextLevel();
}

void game_model::hitProjectile(Projectile *projectile) {
    addExplosion(new Explosion(projectile->getX(), projectile->getY()));
    deleteProjectile(projectile);
}

void game_model::deleteAlien(Alien *alien) {
    if(alien->isDead()) return;
    alien->kill();
}

void game_model::deleteProjectile(Projectile *projectile) {
    if(!projectile) return;
    std::erase(projectiles, projectile);
    delete projectile;
}

void game_model::deleteExplosion(Explosion *explosion) {
    if(!explosion) return;
    std::erase(explosions, explosion);
    delete explosion;
}

void game_model::spawnPowerUp() {
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

void game_model::checkCollisions() {
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

void game_model::checkNextLevel() {
    auto alive = getAliveAliens();
    for (auto & row : alive)
        if (!row.empty())
            return;

    startNextLevel();
}

void game_model::startNextLevel() {
    level++;
    ticks = INITIAL_TICKS;
    timeoutTicks = LEVEL_TIMEOUT;
}

void game_model::gameOver() {
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

#include "game_model.h"

#include <cstdlib>
#include <ncurses.h>
#include <algorithm>

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

// more standard getters
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

// let the player shoot
void GameModel::playerShoot() {
    // if a timeout is present, no shooting action should be taken
    if (timeoutTicks == 0)
    {
        addProjectile(
            new Projectile(
                player.getX(),
                player.getY()-1,
                -5,
                COLOR_WHITE));

        // If a PowerUp is activated shoot more projectiles
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
}

// simulate on game tick
void GameModel::simulate_game_tick()
{
    // nap (else you won't see what hits you...)
    napms(TIME_PER_TICK);

    // check if the health is low enough to cause a game over
    if(player.getHealth() <= 0 && timeoutTicks == 0) {
        gameOver();
    }

    // increase the tick counter
    ticks++;

    // Implement game dynamics.
    notifyUpdate();

    // update game objects except aliens
    updateProjectiles();
    checkCollisions();
    updateExplosions();
    spawnPowerUp();
    player.decreasePowerUpTime();

    // handle the game over and next level timeouts
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

    // update aliens
    updateAliens();

    // reset the tick counter after eight alien steps
    // (because then the alien movement loops)
    if (ticks / TICKS_PER_ALIEN_CHANGE == 8) {
        ticks = 0;
    }
}

// update all aliens
void GameModel::updateAliens()
{
    // aliens have a movement pattern of eight steps
    // one step is made for every TICKS_PER_ALIEN_CHANGE
    int step = ticks / TICKS_PER_ALIEN_CHANGE;

    // iterate over all aliens
    for (int i = 0; i < aliens.size(); i++) {
        for (int j = 0; j < aliens[i].size(); j++) {
            // skip the alien if it's dead
            if (aliens[i][j]->isDead())
                continue;

            // shoot a projectile with a small chance increasing with the level
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
                // on the steps 0, 4 and 8 move the alien down and check if this causes a game over
                if (step == 0 || step == 4 || step == 8) {
                    aliens[i][j]->setY(aliens[i][j]->getY() + 1);
                    if(aliens[i][j]->getY() >= height - 1) {
                        gameOver();
                        return;
                    }
                    continue;
                }
                // on steps 1 to 3 move the alien right
                if (step <= 3) {
                    aliens[i][j]->setX(aliens[i][j]->getX() + 1);
                    continue;
                }
                // on steps 5 to 7 move the alien left
                if (step <= 7) {
                    aliens[i][j]->setX(aliens[i][j]->getX() - 1);
                }
            }
        }
    }
}

// update the state of all current projectiles
void GameModel::updateProjectiles()
{
    std::vector<Projectile*> toRemove = {};

    for (auto & projectile : projectiles) {
        // for lower velocities more ticks are skipped
        if (ticks % projectile->getVelocity() == 0) {
            continue;
        }
        // check if we hit the border and remove the projectile in that case
        if (projectile->getY() <= 5 || projectile->getY() >= height - 1) {
            toRemove.push_back(projectile);
            continue;
        }
        // else move the projectile according to its velocity
        projectile->setY(projectile->getY() + (projectile->getVelocity() > 0 ? 1 : -1));
    }
    for (auto & projectile : toRemove) {
        hitProjectile(projectile);
    }
}

// update the state of all current explosions
void GameModel::updateExplosions() {
    std::vector<Explosion*> toRemove = {};

    for (auto & explosion : explosions) {
        explosion->increaseExplosionState();
        // if the state reaches four remove the explosion
        if(explosion->getExplosionState() == 4) {
            toRemove.push_back(explosion);
        }
    }
    for (auto & explosion : toRemove) {
        deleteExplosion(explosion);
    }
}

// process an alien hit
// increase the score, add an explosion and remove the alien
// also check if the alien was the last and the next level should be started
void GameModel::hitAlien(Alien *alien) {
    score += 1 * level;
    addExplosion(new Explosion(alien->getX(), alien->getY()));
    deleteAlien(alien);
    checkNextLevel();
}

// add an explosion and remove the projectile causing it
void GameModel::hitProjectile(Projectile *projectile) {
    addExplosion(new Explosion(projectile->getX(), projectile->getY()));
    deleteProjectile(projectile);
}

// kill an alien if it's not already dead
void GameModel::deleteAlien(Alien *alien) {
    if(alien->isDead()) return;
    alien->kill();
}

// free memory for a projectile
void GameModel::deleteProjectile(Projectile *projectile) {
    if(!projectile) return;
    std::erase(projectiles, projectile);
    delete projectile;
}

// free memory for an explosion
void GameModel::deleteExplosion(Explosion *explosion) {
    if(!explosion) return;
    std::erase(explosions, explosion);
    delete explosion;
}

// spawn or displace the powerup item with a small random chance
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

// check collisions between game objects
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
        // if it is a player projectile
        if (projectile->getVelocity() < 0) {
            // check if it hits an alien
            for (auto & alienRow : aliens) {
                for (auto & alien : alienRow) {
                    if(!alien->isDead()
                        && alien->getX() == projectile->getX()
                        && alien->getY() == projectile->getY()) {
                        if (std::find(
                            toRemoveProjectiles.begin(),
                            toRemoveProjectiles.end(),
                            projectile) == toRemoveProjectiles.end())
                            toRemoveProjectiles.push_back(projectile);
                        hitAlien(alien);
                    }
                }
            }
            // and check if it hits another projectile
            for (auto & projectileAlien : projectiles) {
                if(projectileAlien->getVelocity() > 0
                    && (projectileAlien->getY() == projectile->getY() || projectileAlien->getY() + 1 == projectile->getY())
                    && projectileAlien->getX() == projectile->getX()) {
                    if (std::find(
                        toRemoveProjectilesAlienProjectileCase.begin(),
                        toRemoveProjectilesAlienProjectileCase.end(),
                        projectile) == toRemoveProjectilesAlienProjectileCase.end())
                        toRemoveProjectilesAlienProjectileCase.push_back(projectile);
                    if (std::find(
                        toRemoveProjectilesAlienProjectileCase.begin(),
                        toRemoveProjectilesAlienProjectileCase.end(),
                        projectileAlien) == toRemoveProjectilesAlienProjectileCase.end())
                        toRemoveProjectilesAlienProjectileCase.push_back(projectileAlien);
                }
            }
        } else {
            // else check if the alien projectile hits the player
            if(projectile->getY() == player.getY()
                && projectile->getX() == player.getX()) {
                if (std::find(
                    toRemoveProjectilesAlienProjectileCase.begin(),
                    toRemoveProjectilesAlienProjectileCase.end(),
                    projectile) == toRemoveProjectilesAlienProjectileCase.end())
                    toRemoveProjectilesAlienProjectileCase.push_back(projectile);
                player.decreaseHealth();
            }
        }
    }

    // remove or explode all colliding projectiles
    for (auto & projectile : toRemoveProjectiles) {
        deleteProjectile(projectile);
    }
    for (auto & projectile : toRemoveProjectilesAlienProjectileCase) {
        hitProjectile(projectile);
    }
}

// checks if the next level should be started
void GameModel::checkNextLevel() {
    // check if any aliens are still alive and if that's the case return without starting the next level
    auto alive = getAliveAliens();
    for (auto & row : alive)
        if (!row.empty())
            return;

    startNextLevel();
}

// called to start the next level
void GameModel::startNextLevel() {
    // increase the level
    level++;
    // reset the current game ticks
    ticks = INITIAL_TICKS;
    // set a timeout after which the next level starts
    timeoutTicks = LEVEL_TIMEOUT;
}

// called when the health reaches zero or an alien reaches the bottom
void GameModel::gameOver() {
    // kill the player (if he has survived till now)
    player.kill();

    // kill all remaining aliens
    for (auto & alienRow : aliens) {
        for (auto & alien : alienRow) {
            deleteAlien(alien);
        }
    }

    // reset all relevant game values to their initial states
    score = INITIAL_SCORE;
    level = INITIAL_LEVEL;
    ticks = INITIAL_TICKS;
    timeoutTicks = GAME_OVER_TIMEOUT;
}

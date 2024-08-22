#include "model_simulator_game.h"
#include <ncurses.h>
#include <bits/algorithmfwd.h>

Player::Player(int y, int x)
{
    setX(x);
    setY(y);
};

int Player::getX() { 
    return x;
};

int Player::getY() { 
    return y;
};

void Player::setX(int a) {
    x = a;
};

void Player::setY(int a) {
    y = a;
};


Alien::Alien()
{
};

int Alien::getX() {
    return x;
};

int Alien::getY() {
    return y;
};

void Alien::setX(int a) {
    x = a;
};

void Alien::setY(int a) {
    y = a;
};

Explosion::Explosion(int x, int y) : x(x), y(y)
{
};

int Explosion::getX() {
    return x;
};

int Explosion::getY() {
    return y;
};

int Explosion::getExplosionState() {
    return explosionState;
}

void Explosion::increaseExplosionState() {
    explosionState++;
}




Projectile::Projectile(int x, int y, int velocity) : x(x), y(y), velocity(velocity)
{

};

int Projectile::getX() {
    return x;
};

int Projectile::getY() {
    return y;
};

int Projectile::getVelocity() {
    return velocity;
};

void Projectile::setX(int a) {
    x = a;
};

void Projectile::setY(int a) {
    y = a;
};


GameModel::GameModel() : player(height-1, width/2)
{
    createAliens();
};

// Example function - used for simple unit tests
int GameModel::addOne(int input_value) {
    return (++input_value); 
};

int GameModel::getGameWidth() {
    return width; 
};
    
int GameModel::getGameHeight() {
    return height; 
};

int GameModel::getScore() {
    return score;
};

std::vector<Projectile*> GameModel::getProjectiles() {
    return projectiles;
}


int GameModel::getLevel() {
    return level;
};
    
Player* GameModel::getPlayer() {
    return &player;
};

std::vector<std::vector<Alien*>>* GameModel::getAliens() {
    return &aliens;
};

void GameModel::increaseScore(int value) {
    score += value;
}


void GameModel::createAliens() {
    for(int i = 0; i < 6; i++) {
        std::vector<Alien*> row = {};
        bool uneven = i % 2 == 0;

        for(int j = 0; j < (uneven ? 9 : 8); j++) {
            auto alien = new Alien();
            alien->setX(j * 4 + (uneven ? 2 : 4));
            alien->setY(i * 2 + 6);
            row.push_back(alien);
        }

        aliens.push_back(row);
    }
};

void GameModel::control_player(wchar_t ch)
{
    if (ch==KEY_LEFT && player.getX() > 1)
    {
        player.setX(player.getX() - 1);
    }
    if (ch==KEY_RIGHT && player.getX() < 38)
    {
        player.setX(player.getX() + 1);
    }
    if (ch==' ')
    {
        addProjectile(new Projectile(player.getX(), player.getY()-1, -1));
    }
};

void GameModel::moveAliens(int step)
{
    for (auto & alienRow : aliens) {
        for (auto & alien : alienRow) {
            if (!alien)
                continue;
            if (step == 0 || step == 4 || step == 8) {
                alien->setY(alien->getY() + 1);
                continue;
            }
            if (step <= 3) {
                alien->setX(alien->getX() + 1);
                continue;
            }
            if (step <= 7) {
                alien->setX(alien->getX() - 1);
            }
        }
    }
}

void GameModel::addProjectile(Projectile* projectile)
{
    projectiles.push_back(projectile);
}


void GameModel::moveProjectiles()
{
    std::vector<Projectile*> toRemove = {};

    for (auto & projectile : projectiles) {
        if (projectile->getY() <= 5 || projectile->getY() >= 30) {
            toRemove.push_back(projectile);
            continue;
        }
        projectile->setY(projectile->getY() + projectile->getVelocity());
    }
    for (auto & projectile : toRemove) {
        projectileHit(projectile);
    }
}

void GameModel::checkCollisions() {
    std::vector<Alien*> toRemoveAliens = {};
    std::vector<Projectile*> toRemoveProjectiles = {};
    std::vector<Projectile*> toRemoveProjectilesAlienProjectileCase = {};

    for (auto & projectile : projectiles) {
        // Projektile vom Spieler
        if (projectile->getVelocity() == -1) {
            // Trifft es ein Alien?
            for (auto & alienRow : aliens) {
                for (auto & alien : alienRow) {
                    if(alien
                        && alien->getX() == projectile->getX()
                        && alien->getY() == projectile->getY()) {
                        toRemoveProjectiles.push_back(projectile);
                        toRemoveAliens.push_back(alien);
                    }
                }
            }
            // Trifft es ein Projektil von einem Alien?
            for (auto & projectileAlien : projectiles) {
                if(projectileAlien->getVelocity() != -1
                            && (projectileAlien->getY() == projectile->getY() || projectileAlien->getY() + 1 == projectile->getY())
                            && projectileAlien->getX() == projectile->getX()) {
                    toRemoveProjectilesAlienProjectileCase.push_back(projectile);
                    toRemoveProjectilesAlienProjectileCase.push_back(projectileAlien);
                }
            }
        } else {
            // Projektile vom Alien
            // TODO:
        }
    }
    for (auto & alien : toRemoveAliens) {
        alienHit(alien);
    }
    for (auto & projectile : toRemoveProjectiles) {
        deleteProjectile(projectile);
    }
    for (auto & projectile : toRemoveProjectilesAlienProjectileCase) {
        projectileHit(projectile);
    }
}

void GameModel::projectileHit(Projectile *projectile) {
    addExplosion(new Explosion(projectile->getX(), projectile->getY()));
    deleteProjectile(projectile);
}


void GameModel::deleteProjectile(Projectile *projectile) {
    if(!projectile) return;
    std::erase(projectiles, projectile);
    delete projectile;
}

std::vector<Explosion *> GameModel::getExplosions() {
    return explosions;
}

void GameModel::addExplosion(Explosion *explosion) {
    explosions.push_back(explosion);
}

void GameModel::removeExplosion(Explosion *explosion) {
    if(!explosion) return;
    std::erase(explosions, explosion);
    delete explosion;
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
        removeExplosion(explosion);
    }
}

void GameModel::deleteAlien(Alien *alien) {
    if(!alien) return;
    for (auto & alienRow : aliens) {
        std::erase(alienRow, alien);
    }
    delete alien;
}

void GameModel::alienHit(Alien *alien) {
    addExplosion(new Explosion(alien->getX(), alien->getY()));
    deleteAlien(alien);
}

void GameModel::simulate_game_step()
{
    time++;
    // Implement game dynamics.
    notifyUpdate();

    moveProjectiles();
    checkCollisions();
    updateExplosions();

    if (time % 40 == 0) {
        moveAliens(time / 40);
        if (time / 40 == 8) {
            time = 0;
        }
    }
}
#include "model_simulator_game.h"

#include <cstdlib>
#include <ncurses.h>
#include <bits/algorithmfwd.h>

// Drawable implementation
Drawable::Drawable(int x, int y) : x(x), y(y) {}

int Drawable::getX() {
    return x;
}
int Drawable::getY() {
    return y;
}
int Drawable::getColor() {
    return color;
}
wchar_t Drawable::getTexture() {
    return texture;
}

void Drawable::setX(int x) {
    this->x = x;
}
void Drawable::setY(int y) {
    this->y = y;
}


// Player implementation
int Player::getColor() {
    return color;
}
wchar_t Player::getTexture() {
    return texture;
}

int Player::getHealth() {
    return health;
}

void Player::decreaseHealth() {
    health--;
}


// Alien implementation
int Alien::getColor() {
    return color;
}
wchar_t Alien::getTexture() {
    return texture;
}
bool Alien::isDead() {
    return dead;
}
void Alien::kill() {
    dead = true;
}




// Explosion implementation
int Explosion::getColor() {
    return color;
}
wchar_t Explosion::getTexture() {
    return texture;
}
short Explosion::getExplosionState() {
    return state;
}

void Explosion::increaseExplosionState() {
    state++;
    switch (state) {
        case 1:
            color = 2;
            texture = '*';
            break;
        case 2:
            color = 3;
            texture = '#';
            break;
        case 3:
            color = 4;
            texture = '@';
            break;
        default:
            break;
    }
}


// Projectile implementation
Projectile::Projectile(int x, int y, int velocity)
    : Drawable(x, y), velocity(velocity)
{};

int Projectile::getVelocity() {
    return velocity;
}
int Projectile::getColor() {
    return color;
}
wchar_t Projectile::getTexture() {
    return texture;
}

void Projectile::setVelocity(int velocity) {
    this->velocity = velocity;
}


// Game model implementation
GameModel::GameModel() : player(width/2, height-1)
{
    createAliens();
};

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
};
std::vector<std::vector<Alien*>> GameModel::getAliens() {
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
void GameModel::addProjectile(Projectile* projectile)
{
    projectiles.push_back(projectile);
};
void GameModel::addExplosion(Explosion *explosion) {
    explosions.push_back(explosion);
};

void GameModel::increaseScore(int value) {
    score += value;
}
void GameModel::increaseLevel() {
    level++;
}

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
        addProjectile(
            new Projectile(
                player.getX(),
                player.getY()-1,
                -5));
    }
};

void GameModel::simulate_game_step()
{
    ticks++;
    // Implement game dynamics.
    notifyUpdate();

    updateProjectiles();
    checkCollisions();
    updateExplosions();
    updateAliens();

    if (ticks / 40 == 8) {
        ticks = 0;
    }
}

// private game model functions
void GameModel::updateAliens()
{
    int step = ticks / 40;
    for (int i = 0; i < aliens.size(); i++) {
        for (int j = 0; j < aliens[i].size(); j++) {
            if (aliens[i][j]->isDead())
                continue;

            // projectiles
            if (i >= aliens.size() - 2
                || (aliens[i+2][j]->isDead()
                    && (i+4 >= aliens.size()
                    || aliens[i+4][j]->isDead()))) {
                bool shoot = rand() % 1001 < 1 * level;
                if (shoot)
                    addProjectile(new Projectile(
                        aliens[i][j]->getX(),
                        aliens[i][j]->getY()+1,
                        1+level));
            }

            // movement every 40 ticks
            if (ticks % 40 == 0) {
                if (step == 0 || step == 4 || step == 8) {
                    aliens[i][j]->setY(aliens[i][j]->getY() + 1);
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
        if (projectile->getY() <= 5 || projectile->getY() >= 30) {
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

void GameModel::checkCollisions() {
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
            // TODO:
        }
    }
    for (auto & projectile : toRemoveProjectiles) {
        deleteProjectile(projectile);
    }
    for (auto & projectile : toRemoveProjectilesAlienProjectileCase) {
        hitProjectile(projectile);
    }
}

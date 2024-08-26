#include "model_simulator_game.h"
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
// Alien implementation
int Alien::getColor() {
    return color;
}
wchar_t Alien::getTexture() {
    return texture;
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
std::vector<std::vector<Alien*>>* GameModel::getAliens() {
    return &aliens;
};
std::vector<Projectile*> GameModel::getProjectiles() {
    return projectiles;
};
std::vector<Explosion *> GameModel::getExplosions() {
    return explosions;
}

void GameModel::createAliens() {
    // TODO: Implement levels
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
                -1));
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

    // aliens only move every 40 ticks
    if (ticks % 40 == 0) {
        updateAliens(ticks / 40);
        if (ticks / 40 == 8) {
            ticks = 0;
        }
    }
}

// private game model functions
void GameModel::updateAliens(int step)
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

void GameModel::updateProjectiles()
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
    if(!alien) return;
    for (auto & alienRow : aliens) {
        std::erase(alienRow, alien);
    }
    delete alien;
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
        hitAlien(alien);
    }
    for (auto & projectile : toRemoveProjectiles) {
        deleteProjectile(projectile);
    }
    for (auto & projectile : toRemoveProjectilesAlienProjectileCase) {
        hitProjectile(projectile);
    }
}

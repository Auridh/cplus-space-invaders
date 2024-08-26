#include "model_simulator_game.h"

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
    if(health > 0)
        health--;
}

void Player::resetHealth() {
    health = 3;
}

void Player::kill() {
    health = 0;
}

int Player::getPowerUpTime() {
    return powerUpTime;
}


void Player::takePowerUp(short type) {
    switch (type) {
        case 0:
            // PowerUpTime in ticks
            powerUpTime = 120;
            break;
        default:
            health++;
            break;
    }

}

void Player::decreasePowerUpTime() {
    if(powerUpTime == 0) {
        return;
    }
    powerUpTime--;
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

void Alien::revivify(int x, int y) {
    dead = false;
    setX(x);
    setY(y);
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
            color = 4;
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
Projectile::Projectile(int x, int y, int velocity, int color)
    : Drawable(x, y), velocity(velocity), color(color)
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


short PowerUp::getType() {
    return type;
}

void PowerUp::setType(short type) {
    this->type = type;
    switch (type) {
        case 1:
            texture = '+';
            color = 2;
            break;
        default:
            texture = '$';
            color = 4;
            break;
    }
}

int PowerUp::getColor() {
    return color;
}

wchar_t PowerUp::getTexture() {
    return texture;
}


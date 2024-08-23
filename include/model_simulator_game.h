// header guard to prevent multiple inclusions of the same header file
#ifndef MODEL_GAME_H_
#define MODEL_GAME_H_

// include header file for the Observable class
#include "observer.h"

// Super class vor all game objects
class Drawable {
public:
    Drawable() = default;
    Drawable(int x, int y);
    virtual ~Drawable() = default;
    virtual int getX();
    virtual int getY();
    virtual int getColor();
    virtual wchar_t getTexture();
    void setX(int x);
    void setY(int y);
private:
    int x = 0, y = 0;
    wchar_t texture = ' ';
    int color = 1;
};

class Player : public Drawable {
    using Drawable::Drawable;
    wchar_t texture = 'A';
    int color = 2;
public:
    int getColor() override;
    wchar_t getTexture() override;
};

class Alien : public Drawable {
    using Drawable::Drawable;
    wchar_t texture = 'Y';
    int color = 1;
public:
    int getColor() override;
    wchar_t getTexture() override;
};

class Explosion : public Drawable {
    using Drawable::Drawable;
    wchar_t texture = '*';
    short state = 0;
    int color = 2;
public:
    int getColor() override;
    wchar_t getTexture() override;
    short getExplosionState();
    void increaseExplosionState();
};

class Projectile : public Drawable {
    using Drawable::Drawable;
    int velocity = 1;
    wchar_t texture = '.';
    int color = 2;
public:
    Projectile(int x, int y, int velocity);
    int getVelocity();
    int getColor() override;
    wchar_t getTexture() override;
    void setVelocity(int value);
};


// Game class inherits from Observable class
class GameModel : public Observable {
public:
    GameModel();

    int getGameWidth();
    int getGameHeight();
    int getLevel();
    int getScore();

    Player* getPlayer();
    std::vector<std::vector<Alien*>>* getAliens();
    std::vector<Projectile*> getProjectiles();
    std::vector<Explosion*> getExplosions();

    // create all aliens according to the current level
    void createAliens();
    // add objects to the game world
    void addProjectile(Projectile* projectile);
    void addExplosion(Explosion* explosion);

    // control meta information
    void increaseScore(int value);
    void increaseLevel();

    // updates player movement direction based on keyboard input
    void control_player(wchar_t ch);
    // simulates one step of the game
    void simulate_game_step();

private:
    // game width and height
    int width = 40, height = 27;
    int level = 1, score = 0;
    // time measurement
    int ticks = 0;

    // game objects
    Player player;
    std::vector<std::vector<Alien*>> aliens = {};
    std::vector<Projectile*> projectiles = {};
    std::vector<Explosion*> explosions = {};

    // update game objects
    void updateAliens(int step);
    void updateProjectiles();
    void updateExplosions();
    void hitAlien(Alien* alien);
    void hitProjectile(Projectile* projectile);
    void deleteAlien(Alien* alien);
    void deleteProjectile(Projectile* projectile);
    void deleteExplosion(Explosion* explosion);
    // check for object collisions
    void checkCollisions();
};

#endif

#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "drawable.h"
#include "observer.h"

// Game class inherits from Observable class
class GameModel : public Observable {
    // milliseconds per second
    const int TIME_PER_SECOND = 1000;
public:
    // time the game naps each tick in milliseconds
    const int TIME_PER_TICK = 25;
    const int TICKS_PER_SECOND = TIME_PER_SECOND / TIME_PER_TICK;
    // timeout after a level ends or at game start
    const int LEVEL_TIMEOUT = 3 * TICKS_PER_SECOND;
    // timeout after gameover
    const int GAME_OVER_TIMEOUT = 2 * LEVEL_TIMEOUT;
    // ticks for next alien movement step
    const int TICKS_PER_ALIEN_CHANGE = 2 * TICKS_PER_SECOND;
    // initial values
    const int INITIAL_SCORE = 0,
              INITIAL_LEVEL = 1,
              INITIAL_TICKS = 0,
              INITIAL_TIMEOUT = LEVEL_TIMEOUT;

    // Constructor
    // initializes the player and aliens
    GameModel();
    ~GameModel();

    // getters for global values
    int getGameWidth();
    int getGameHeight();
    int getLevel();
    int getScore();
    int getTimeout();

    // getters for game objects
    Player* getPlayer();
    PowerUp* getPowerUp();
    std::vector<std::vector<Alien*>> getAliveAliens();
    std::vector<Projectile*> getProjectiles();
    std::vector<Explosion*> getExplosions();

    // create all aliens according to the current level
    void createAliens();
    void spawnAliens();
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
    int level = INITIAL_LEVEL,
        score = INITIAL_SCORE;
    // time measurement
    int ticks = INITIAL_TICKS;
    // ticks till the game resumes
    int timeoutTicks = INITIAL_TIMEOUT;

    // game objects
    Player player;
    PowerUp* powerUp;
    std::vector<std::vector<Alien*>> aliens = {};
    std::vector<Projectile*> projectiles = {};
    std::vector<Explosion*> explosions = {};

    // update game objects
    void updateAliens();
    void updateProjectiles();
    void updateExplosions();
    void hitAlien(Alien* alien);
    void hitProjectile(Projectile* projectile);
    void deleteAlien(Alien* alien);
    void deleteProjectile(Projectile* projectile);
    void deleteExplosion(Explosion* explosion);
    void spawnPowerUp();
    // check for object collisions
    void checkCollisions();
    // level system
    void checkNextLevel();
    void startNextLevel();
    void gameOver();
};

#endif //GAMEMODEL_H

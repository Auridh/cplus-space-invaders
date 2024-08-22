#ifndef MODEL_GAME_H_ // header guard to prevent multiple inclusions of the same header file
#define MODEL_GAME_H_

#include "observer.h" // include header file for the Observable class

class Player {
public:
    Player(int x, int y); // constructor that takes in initial x and y coordinates of player
    int getX();
    int getY();
    void setX(int a);
    void setY(int a);
private:
    int x, y; // player's coordinates and height
};

class Alien {
public:
    Alien(); // constructor that takes in initial x and y coordinates of player
    int getX();
    int getY();
    void setX(int a);
    void setY(int a);
private:
    int x, y; // player's coordinates and height
};

class Explosion {
public:
    Explosion(int x, int y); // constructor that takes in initial x and y coordinates of player
    int getX();
    int getY();
    int getExplosionState();

    void increaseExplosionState();
private:
    int x, y; // player's coordinates and height
    int explosionState = 0;
};

class Projectile {
public:
    Projectile(int x, int y, int velocity); // constructor that takes in initial x and y coordinates of player
    int getX();
    int getY();
    int getVelocity();
    void setX(int a);
    void setY(int a);
    void setVelocity(int value);
private:
    int x, y, velocity; // player's coordinates and height
};

class GameModel : public Observable { // Game class inherits from Observable class
public:
    GameModel(); // constructor

    int getGameWidth(); // returns the game's width
    int getGameHeight(); // returns the game's height
    Player& getPlayer(); // returns reference to player object
    int getLevel();
    std::vector<std::vector<Alien*>>* getAliens();
    int getScore();
    std::vector<Projectile*> getProjectiles();
    std::vector<Explosion*> getExplosions();

    void simulate_game_step(); // simulates one step of the game
    void control_player(wchar_t ch); // updates player movement direction based on keyboard input

    int addOne(int input_value); // Example function - used for simple unit tests
    void increaseScore(int value);
    void nextLevel();
    void createAliens();
    void moveAliens(int step);
    void moveProjectiles();
    void addProjectile(Projectile* projectile);
    void checkCollisions();
    void deleteProjectile(Projectile* projectile);
    void projectileHit(Projectile* projectile);
    void addExplosion(Explosion* explosion);
    void removeExplosion(Explosion* explosion);
    void updateExplosions();
    void deleteAlien(Alien* alien);
    void alienHit(Alien* alien);

private:
    int width = 40; // game width
    int height = 27; // game height
    int dir = 1; // ball direction
    int level = 1;
    int score = 0;
    int time = 0;
    Player player; // player object
    std::vector<std::vector<Alien*>> aliens = {};
    std::vector<Projectile*> projectiles = {};
    std::vector<Explosion*> explosions = {};
};

#endif // end of header file

// header guard to prevent multiple inclusions of the same header file
#ifndef MODEL_GAME_H_
#define MODEL_GAME_H_
#include <ncurses.h>

// super class for all game objects
class Drawable {
public:
    // default constructor
    Drawable() = default;
    // constructor initializing x and y positions
    Drawable(int x, int y);
    // default destructor
    virtual ~Drawable() = default;
    // standard getters are virtual vor subclass compatibility
    virtual int getX();
    virtual int getY();
    virtual int getColor();
    virtual wchar_t getTexture();
    // setters for the x and y positions
    void setX(int x);
    void setY(int y);
private:
    // x and y coordinates
    int x = 0, y = 0;
    // texture/ASCII-character which should be used for the model
    wchar_t texture = '?';
    // color the
    int color = COLOR_WHITE;
};

// player character
class Player : public Drawable {
    // using statement to make Drawable constructors valid for the Player class
    using Drawable::Drawable;
    wchar_t texture = 'A';
    int color = COLOR_WHITE;
    int health = 3;
    // ticks the player is still powered up
    int powerUpTime = 0;
public:
    // overrides for getting subclass color and texture attributes
    int getColor() override;
    wchar_t getTexture() override;
    // get the players remaining health
    int getHealth();
    // decrease the players health by one
    void decreaseHealth();
    // reset the health to three
    void resetHealth();
    // kill the player setting its health to zero
    void kill();
    // get the remaining the player is in a powered up state
    int getPowerUpTime();
    // put the player in a powered up state
    void takePowerUp(short type);
    // decrease the remaining powered up time by one tick
    void decreasePowerUpTime();
};

// aliens
class Alien : public Drawable {
    // using statement to make Drawable constructors valid for the Alien class
    using Drawable::Drawable;
    wchar_t texture = 'Y';
    int color = COLOR_GREEN;
    bool dead = true;
public:
    // overrides for getting subclass color and texture attributes
    int getColor() override;
    wchar_t getTexture() override;
    // check if the alien is dead
    bool isDead();
    // set the aliens state to dead
    void kill();
    // resurrect the alien at a given position
    void revivify(int x, int y);
};

// explosions
class Explosion : public Drawable {
    // using statement to make Drawable constructors valid for the Explosion class
    using Drawable::Drawable;
    wchar_t texture = '*';
    short state = 0;
    int color = COLOR_YELLOW;
public:
    // overrides for getting subclass color and texture attributes
    int getColor() override;
    wchar_t getTexture() override;
    // get and increase the state of the explosion
    // (used for animating explosions)
    short getExplosionState();
    void increaseExplosionState();
};

// projectiles
class Projectile : public Drawable {
    // using statement to make Drawable constructors valid for the Projectile class
    using Drawable::Drawable;
    int velocity = 10;
    wchar_t texture = '.';
    int color = COLOR_GREEN;
public:
    // constructor with extra velocity and color for alien and player projectiles
    Projectile(int x, int y, int velocity, int color);
    // overrides for getting subclass color and texture attributes
    int getColor() override;
    wchar_t getTexture() override;
    // get the projectiles velocity
    int getVelocity();
    // set the projectiles velocity
    void setVelocity(int value);
};

// power-ups
class PowerUp : public Drawable {
    // using statement to make Drawable constructors valid for the PowerUp class
    using Drawable::Drawable;
    short type = 0;
    wchar_t texture = '$';
    int color = COLOR_YELLOW;
public:
    // overrides for getting subclass color and texture attributes
    int getColor() override;
    wchar_t getTexture() override;
    // setter and getter for the projectile's type
    short getType();
    void setType(short type);
};

#endif

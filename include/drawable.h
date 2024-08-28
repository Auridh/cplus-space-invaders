// header guard to prevent multiple inclusions of the same header file
#ifndef MODEL_GAME_H_
#define MODEL_GAME_H_

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
    int color = 1;
    int health = 3;
    int powerUpTime = 0;
public:
    int getColor() override;
    wchar_t getTexture() override;
    int getHealth();
    void decreaseHealth();
    void resetHealth();
    void kill();
    int getPowerUpTime();
    void takePowerUp(short type);
    void decreasePowerUpTime();
};

class Alien : public Drawable {
    using Drawable::Drawable;
    wchar_t texture = 'Y';
    int color = 5;
    bool dead = true;
public:
    int getColor() override;
    wchar_t getTexture() override;
    bool isDead();
    void kill();
    void revivify(int x, int y);
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
    int velocity = 10;
    wchar_t texture = '.';
    int color = 5;
public:
    Projectile(int x, int y, int velocity, int color);
    int getVelocity();
    int getColor() override;
    wchar_t getTexture() override;
    void setVelocity(int value);
};

class PowerUp : public Drawable {
    using Drawable::Drawable;
    short type = 0;
    wchar_t texture = '$';
    int color = 4;
public:
    short getType();
    void setType(short type);
    int getColor() override;
    wchar_t getTexture() override;
};

#endif

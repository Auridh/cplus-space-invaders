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
    int x, y, height; // player's coordinates and height
};

class Alien {
public:
    Alien(); // constructor that takes in initial x and y coordinates of player
    int getX();
    int getY();
    void setX(int a);
    void setY(int a);
private:
    int x, y, height; // player's coordinates and height
};

class GameModel : public Observable { // Game class inherits from Observable class
public:
    GameModel(); // constructor

    int getGameWidth(); // returns the game's width
    int getGameHeight(); // returns the game's height
    Player& getPlayer(); // returns reference to player object
    int getLevel();
    Alien*** getAliens();
    int getScore();

    void simulate_game_step(); // simulates one step of the game
    void control_player(wchar_t ch); // updates player movement direction based on keyboard input

    int addOne(int input_value); // Example function - used for simple unit tests
    void increaseScore(int value);
    void nextLevel();
    void createAliens();
    void moveAliens(int step);

private:
    int width = 40; // game width
    int height = 27; // game height
    int dir = 1; // ball direction
    int level = 1;
    int score = 0;
    int time = 0;
    Player player; // player object
    Alien*** aliens;
};

#endif // end of header file

#include "model_simulator_game.h"
#include <ncurses.h>
#include <stdlib.h>

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


GameModel::GameModel() : player(height, width/2)
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

int GameModel::getLevel() {
    return level;
};
    
Player& GameModel::getPlayer() {
    return player; 
};

Alien** GameModel::getAliens() {
    return aliens;
};

void GameModel::createAliens() {
    Alien** table = new Alien*[6];

    for(int i = 0; i < 6; i++) {
        table[i] = new Alien[12];
        for(int j = 0; j < 12; j++) {
            table[i][j] = Alien();
            aliens[i][j].setX(j * 4 + 1);
            aliens[i][j].setY(i * 2 + 2);
        }
    }

    aliens = table;
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
};

void GameModel::simulate_game_step()
{
    // Implement game dynamics.
    notifyUpdate();
};
#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>

#include "game_model.h"

BOOST_AUTO_TEST_SUITE(GameModelTests);

BOOST_AUTO_TEST_CASE(ScoreIncrease)
{
    auto game = new GameModel();
    // at the start the score should be zero
    BOOST_CHECK_EQUAL(game->INITIAL_SCORE, game->getScore());
    game->increaseScore(17);
    // after the increase of seventeen it should be seventeen
    BOOST_CHECK_EQUAL(game->INITIAL_SCORE+17, game->getScore());
    game->increaseScore(5);
    // and now it obviously should be seventeen plus five
    BOOST_CHECK_EQUAL(game->INITIAL_SCORE+22, game->getScore());
}

BOOST_AUTO_TEST_CASE(LevelIncrease)
{
    auto game = new GameModel();
    // at the start the level should be one
    BOOST_CHECK_EQUAL(game->INITIAL_LEVEL, game->getLevel());
    game->increaseLevel();
    // now it should have increased by one
    BOOST_CHECK_EQUAL(game->INITIAL_LEVEL + 1, game->getLevel());
    game->increaseLevel();
    // and now again
    BOOST_CHECK_EQUAL(game->INITIAL_LEVEL + 2, game->getLevel());
}

BOOST_AUTO_TEST_CASE(GameOver)
{
    auto game = new GameModel();

    // check that all values are set as expected
    BOOST_CHECK_EQUAL(game->INITIAL_SCORE, game->getScore());
    BOOST_CHECK_EQUAL(game->INITIAL_LEVEL, game->getLevel());
    BOOST_CHECK_EQUAL(game->INITIAL_TIMEOUT, game->getTimeout());
    BOOST_CHECK_EQUAL(0, game->getAliveAliens()[0].size());
    BOOST_CHECK_EQUAL(3, game->getPlayer()->getHealth());

    // simulate the players death
    for (int i = 0; i < game->INITIAL_TIMEOUT; i++)
        game->simulate_game_tick();
    game->getPlayer()->decreaseHealth();
    game->getPlayer()->decreaseHealth();
    game->getPlayer()->decreaseHealth();
    game->simulate_game_tick();

    // check if the game over function was correctly executed
    BOOST_CHECK_EQUAL(game->INITIAL_SCORE, game->getScore());
    BOOST_CHECK_EQUAL(game->INITIAL_LEVEL, game->getLevel());
    BOOST_CHECK_EQUAL(game->GAME_OVER_TIMEOUT - 1, game->getTimeout());
    BOOST_CHECK_EQUAL(0, game->getAliveAliens()[0].size());
    BOOST_CHECK_EQUAL(0, game->getPlayer()->getHealth());
}

BOOST_AUTO_TEST_SUITE_END()
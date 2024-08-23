#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>

#include <ncurses.h>
#include "model_simulator_game.h"

BOOST_AUTO_TEST_SUITE(AddOneTest);

BOOST_AUTO_TEST_CASE(TestCases)
{
    GameModel* game = new GameModel();
    BOOST_CHECK_EQUAL(21, 21);
}

BOOST_AUTO_TEST_SUITE_END()
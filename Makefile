CXX	:= c++
CXXFLAGS := -Wall -std=c++20

# Contain path for any includes (headers)
# Depending on your platform: Include a path to boost, on linux should be 
# /usr/local/include, on mac could be /opt/homebrew/include
INCLUDES := -I./include -I/opt/homebrew/include

# Contains libraries we need to (-L is directory search path, -l is lib)
LDFLAGS = -L/usr/local/lib -L/opt/homebrew/lib 
LDLIBS = -lncurses -lboost_unit_test_framework

SRCDIR := ./src
TESTDIR := ./tests
GAME_OBJECTS := controller_console.o drawable.o observer.o view_console.o game_model.o

game: $(GAME_OBJECTS) main.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

test: $(GAME_OBJECTS) test_call.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

test_call.o: $(TESTDIR)/test_call.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@

clean:
	test ! -f game || rm game
	test ! -f test || rm "test"
	rm -f *.o
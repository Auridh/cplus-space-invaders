#ifndef OBSERVER_H_ // header guard to prevent multiple inclusions of the header file
#define OBSERVER_H_

#include <vector> // include the vector standard library, which will be used to store pointers to observers

class Observer // define the Observer class
{
public:
    virtual void update() = 0; // declare a pure virtual function called update(), which will be implemented by derived classes
};

class Observable // define the Observable class
{
    std::vector<Observer*> observers; // private vector to store pointers to registered observers

public:
    void addObserver(Observer* observer); // public function to add an observer to the vector of registered observers

    void notifyUpdate(); // public function to notify all registered observers that an update has occurred
};

#endif // end of header guard and file

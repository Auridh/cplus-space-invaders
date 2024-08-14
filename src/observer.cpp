#include "observer.h"
#include <vector>

void Observable::addObserver(Observer* observer) {
	observers.push_back(observer);
};

void Observable::notifyUpdate() {
	for (auto& observer : observers){
		observer->update();
	}
};
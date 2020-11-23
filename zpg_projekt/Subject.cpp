#include "Subject.hpp"
#include "Shader.hpp"


void Subject::registerObserver(std::shared_ptr<Observer> observer) {
	observers.push_back(observer);
}

void Subject::removeObserver(std::shared_ptr<Observer> observer) {
	// find the observer
	auto iterator = std::find(observers.begin(), observers.end(), observer);

	if (iterator != observers.end()) {	// observer found
		observers.erase(iterator);		// remove the observer
	}
}

/*void Subject::registerObserver(Observer& observer) {
	observers.push_back(&observer);
}

void Subject::removeObserver(Observer& observer) {
	// find the observer
	auto iterator = std::find(observers.begin(), observers.end(), &observer);

	if (iterator != observers.end()) {	// observer found
		observers.erase(iterator);		// remove the observer
	}
}*/

// predavani subjectu misto stringu
/*void Subject::notifyObservers(std::string change) {
	for (auto observer : observers) {  // notify all observers

		if (observer == nullptr)
		{
			printf("something wrong here [notifyObservers()]!\n");
			//return;
		}

		observer->update(change);
	}
}*/

void Subject::notifyObservers(Camera* camera, camChange cameraChange) {	//Subject*
	for (auto observer : observers) {  // notify all observers
		if (observer == nullptr)
		{
			printf("Something wrong here [notifyObservers()]!\n");
			//return;
		}
		
		observer->update(camera, cameraChange);
	}
}


/*for(Observer* observer : observers){
	if (change == "projection")
		continue;*/
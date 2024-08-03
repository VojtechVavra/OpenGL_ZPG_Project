#include "Subject.hpp"
#include "Shader.hpp"
#include "Camera.hpp"


void Subject::registerObserver(std::shared_ptr<IObserver> observer) {
	m_observers.push_back(observer);
}

void Subject::removeObserver(std::shared_ptr<IObserver> observer) {
	m_observers.remove(observer);
}

void Subject::notifyObservers(Camera* camera, camChange cameraChange) {
	for (auto it = m_observers.begin(); it != m_observers.end(); it++) {

		// TODO: predelat aby funkce notifyObservers nebrala 2 specificke
		// parametry, ale aby vzala lambda vyraz a ten se pak
		// vlozil do parametru: (*it)->update(function);

		(*it)->update(camera, cameraChange);
	}
}

/*void Subject::notifyObservers(Camera* camera, camChange cameraChange) {
	for (auto it = m_observers.begin(); it != m_observers.end(); it++) {
		(*it)->update(camera, cameraChange);
	}
}*/

/*void Subject::notifyObservers(Camera* camera, camChange cameraChange) {
	for (auto observer : m_observers) {  // notify all observers
		if (observer == nullptr)
		{
			printf("Something wrong here [notifyObservers()]!\n");
			//return;
		}
		
		observer->update(camera, cameraChange);
	}
}*/

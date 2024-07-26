#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <string>

//#include "Camera.hpp"
//#include "Subject.hpp"

//class Camera;
//enum class camChange;

class Observer
{ // z tohoto dedi Object a z Objectu Camera
public:
	// predelat na identifikator misto stringu
	// predat subject sam sebe
	//virtual void update(std::string change) = 0;
	virtual void update(class Camera* camera, enum class camChange cameraChange) = 0;
private:
	
};

#endif //OBSERVER_HPP
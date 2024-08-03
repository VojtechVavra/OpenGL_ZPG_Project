#ifndef OBSERVER_HPP
#define OBSERVER_HPP


class IObserver
{ // z tohoto dedi Object a z Objectu Camera
public:
	// predat subject sam sebe
	//virtual void update(std::string change) = 0;
	virtual void update(class Camera* camera, enum class camChange cameraChange) = 0;
};

#endif //OBSERVER_HPP

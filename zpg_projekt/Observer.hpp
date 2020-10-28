#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <string>

class Observer
{
public:
	virtual void update(std::string change) = 0;
	//void update(std::string change);
private:
	
};

#endif //OBSERVER_HPP
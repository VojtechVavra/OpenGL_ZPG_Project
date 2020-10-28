#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <vector>
#include <string>
#include <memory>

#include "Observer.hpp"


class Subject
{
public:
	//void registerObserver(Observer& observer);
	void registerObserver(std::shared_ptr<Observer> observer);
	//void removeObserver(Observer* observer);
	void removeObserver(std::shared_ptr<Observer> observer);
	void notifyObservers();	// Notify all the registered observers when a change happens
	void notifyObservers(std::string change);
private:
	//std::vector<Observer*> observers;
	std::vector<std::shared_ptr<Observer>> observers;
};

#endif //SUBJECT_HPP
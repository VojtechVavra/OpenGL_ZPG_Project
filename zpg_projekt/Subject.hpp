#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <list>
#include <string>
#include <memory>
#include <functional>

#include "IObserver.hpp"

/**
 * The Subject owns some important state and notifies observers when the state
 * changes.
 */
class Subject
{
public:
	void registerObserver(std::shared_ptr<IObserver> observer);
	void removeObserver(std::shared_ptr<IObserver> observer);
	// Notify all the registered observers when a change happens
	void notifyObservers(Camera* camera, enum class camChange cameraChange);
private:
	//std::vector<Observer*> observers;
	std::list<std::shared_ptr<IObserver>> m_observers;
	//std::vector<std::shared_ptr<Observer>> observers;
};

#endif // !SUBJECT_HPP
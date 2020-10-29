#ifndef MODELPREFABS_HPP
#define MODELPREFABS_HPP

#include <memory>
#include <unordered_map>
#include <string>

#include "Model.hpp"

class ModelPrefabs
{
public:
	ModelPrefabs();
	Model getModel(std::string modelName);
	~ModelPrefabs();
private:
	std::unordered_map<std::string, Model> modelPrefabs;
	// Disallow creating an instance of this object - not now, not here
};

#endif // !MODELPREFABS_HPP
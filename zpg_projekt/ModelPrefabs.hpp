#ifndef MODELPREFABS_HPP
#define MODELPREFABS_HPP

#include <memory>
#include <unordered_map>
#include <string>

#include "Model.hpp"

class ModelPrefabs
{
public:
	static std::shared_ptr<ModelPrefabs> getInstance();
	
	Model getModel(std::string modelName);
	~ModelPrefabs();
private:
	ModelPrefabs();
	static std::shared_ptr<ModelPrefabs> instance;
	
	std::unordered_map<std::string, Model> modelPrefabs;
};

#endif // !MODELPREFABS_HPP
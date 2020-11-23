#include <iostream>

#include "ModelPrefabs.hpp"

//std::unordered_map<std::string, Model> ModelPrefabs::

ModelPrefabs::ModelPrefabs()
{
	this->modelPrefabs = {
		{ "plain",		 Model("plain")		  },
		{ "sphere",		 Model("sphere")	  },
		{ "suzi_smooth", Model("suzi_smooth") },
		{ "suzi_flat",	 Model("suzi_flat")	  },
		{ "sphere",		 Model("sphere")	  },

		{ "bedna",		 Model("bedna")		  },
		{ "worker",		 Model("worker")	  }
	};
}

Model ModelPrefabs::getModel(std::string modelName)
{
	// If key not found in map iterator to end is returned 
	if (modelPrefabs.find(modelName) == modelPrefabs.end()) {
		return modelPrefabs[std::string("sphere")];
		//std::cout << modelName << " not found\n";
	}
	// else when we found it, then iterator to that key is returned
		//std::cout << "Found " << modelName << "\n";
		
	return modelPrefabs[modelName];
}

ModelPrefabs::~ModelPrefabs()
{
	std::cout << "Destructor of ModelPrefabs\n";
}

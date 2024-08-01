#pragma once

#include <unordered_map>
#include <memory>

template <typename DataType>
class TBaseCollectionManager
{
public:
	TBaseCollectionManager() = default;
	std::shared_ptr<DataType> getData(std::string name, bool createIfNotExist = true);
	void addData(std::shared_ptr<DataType> newData);
	void deleteAll();
	void deleteOne(const std::string& name);
	void deleteOne(std::shared_ptr<DataType> data);

private:
	//std::unordered_map<DataType> m_data;
	// std::unordered_map<std::string, DataType> m_data;
	std::vector<std::shared_ptr<DataType>> m_data;
};

template<typename DataType>
inline std::shared_ptr<DataType> TBaseCollectionManager<DataType>::getData(std::string dataName, bool createIfNotExist)
{
	//auto it = m_data.find(dataName);
	auto it = std::find_if(m_data.begin(), m_data.end(), [&dataName](const std::shared_ptr<DataType>& data) {
		return *data == dataName;
	});

	if (it != m_data.end()) {
		return *it;
	}
	else if (createIfNotExist) {
		//auto newData = std::make_shared<DataType>(dataName);
		//m_data.push_back(newData);
		//return newData;
	}

	//return *it != nullptr ? *it : nullptr;
	/*if (it != m_data.end()) {
		return *it;
	}*/

	return nullptr;
}

template<typename DataType>
inline void TBaseCollectionManager<DataType>::addData(std::shared_ptr<DataType> newData)
{
	std::string newName = "dwadaw";

	m_data.push_back(newData);

	//m_data.insert({ newName, newData});
	//return std::shared_ptr<DataType>();
}

template<typename DataType>
inline void TBaseCollectionManager<DataType>::deleteAll()
{
	m_data.clear();
}

template<typename DataType>
inline void TBaseCollectionManager<DataType>::deleteOne(const std::string& name)
{
	deleteOne(getData(name, false));
}

template<typename DataType>
inline void TBaseCollectionManager<DataType>::deleteOne(std::shared_ptr<DataType> dataToDelete)
{
	// Odstranit všechny výskyty hodnoty dataToDelete
	m_data.erase(std::remove(m_data.begin(), m_data.end(), dataToDelete), m_data.end());
}

#include "DataManager.h"
#include "DataManager.hpp"

#include <algorithm>
#include <DataStorage.hpp>
#include "IDataStorage.h"

DataManager::DataManager::DataManager() : m_dataStorage(DataStorage::dataStorage()), m_categories(std::make_shared<Categories>()) {}

void DataManager::DataManager::setListener(Listener *listener) {
    if (m_listener != listener) {
        m_listener = listener;
    }
}

OpenProjectResult DataManager::DataManager::openProject(const std::string& fileName) {
    auto result = m_dataStorage->openProject(fileName);
    if (result != OpenProjectResult::Error) {
        if (m_dataStorage->loadCategories(m_categories)) {
        }
    }
    return result;
}

std::shared_ptr<Categories> DataManager::DataManager::categories() {
    return m_categories;
}

void DataManager::DataManager::appendCategory(Category category) {
    int id;
    if (m_dataStorage->appendCategory(category, id)) {
        category.id = id;
        m_categories->push_back(category);
        if (m_listener) {
            m_listener->onCategoryAppend();
        }
    }
}

void DataManager::DataManager::renameCategory(const std::string& name, int id) {
    if (auto category = std::find_if(m_categories->begin(), m_categories->end(), [id](const Category& category) { return category.id == id; });
        category != m_categories->end() && m_dataStorage->renameCategory(name, id)) {
        category->name = name;
        if (m_listener) {
            m_listener->onCategoryRenamed(id);
        }
    }
}

std::shared_ptr<DataManager::IDataManager> DataManager::dataManager() {
    static std::shared_ptr<DataManager::IDataManager> manager = std::make_shared<DataManager>();
    return manager;
}

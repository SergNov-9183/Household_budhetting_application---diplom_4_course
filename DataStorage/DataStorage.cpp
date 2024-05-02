#include "DataStorage.h"
#include "DataStorage.hpp"

#include "SqlCategories.h"

DataStorage::DataStorage::DataStorage() {}

OpenProjectResult DataStorage::DataStorage::openProject(const std::string& fileName) {
    auto result = m_db.open(fileName);
    if (result == OpenProjectResult::Created || result == OpenProjectResult::Opened) {
        m_categories.reset(new SqlCategories(m_db));
        if (result == OpenProjectResult::Created && !m_categories->create()) {
            m_db.close();
            return OpenProjectResult::Error;
        }
    }
    return result;
}

bool DataStorage::DataStorage::loadCategories(std::shared_ptr<Categories>& categories) {
    return m_categories && m_categories->select(categories);
}

bool DataStorage::DataStorage::appendCategory(const Category& category, int& id) {
    if (m_categories && m_categories->insert(category.name, category.level, category.parentId, category.income)) {
        id = m_categories->lastId();
        return true;
    }
    id = -1;
    return false;
}

bool DataStorage::DataStorage::renameCategory(const std::string& name, int id) {
    return m_categories && m_categories->updateName(name, id);
}

std::shared_ptr<DataStorage::IDataStorage> DataStorage::dataStorage() {
    static std::shared_ptr<DataStorage::IDataStorage> storage = std::make_shared<DataStorage>();
    return storage;
}

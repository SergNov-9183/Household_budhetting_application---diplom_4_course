#include "DataStorage.h"
#include "DataStorage.hpp"

#include "SqlCategories.h"
#include "SqlAccounts.h"

DataStorage::DataStorage::DataStorage() {}

OpenProjectResult DataStorage::DataStorage::openProject(const std::string& fileName) {
    auto result = m_db.open(fileName);
    if (result == OpenProjectResult::Created || result == OpenProjectResult::Opened) {
        m_categories.reset(new SqlCategories(m_db));
        m_accounts.reset(new SqlAccounts(m_db));
        if (result == OpenProjectResult::Created && !(m_categories->create() && m_accounts->create())) {
            m_db.close();
            return OpenProjectResult::Error;
        }
    }
    return result;
}

bool DataStorage::DataStorage::loadCategories(std::shared_ptr<Categories>& categories) {
    return m_categories && m_categories->select(categories);
}

bool DataStorage::DataStorage::loadAccounts(std::shared_ptr<Accounts>& accounts) {
    return m_accounts && m_accounts->select(accounts);
}

bool DataStorage::DataStorage::appendCategory(const Category& category, int& id) {
    if (m_categories && m_categories->insert(category.name, category.level, category.parentId, category.income)) {
        id = m_categories->lastId();
        return true;
    }
    id = -1;
    return false;
}

bool DataStorage::DataStorage::appendAccount(const Account& account, int& id) {
    if (m_accounts && m_accounts->insert(account.name, account.type, account.parentId)) {
        id = m_accounts->lastId();
        return true;
    }
    id = -1;
    return false;
}

bool DataStorage::DataStorage::renameCategory(const std::string& name, int id) {
    return m_categories && m_categories->updateName(name, id);
}

bool DataStorage::DataStorage::renameAccount(const std::string& name, int id) {
    return m_accounts && m_accounts->updateName(name, id);
}

std::shared_ptr<DataStorage::IDataStorage> DataStorage::dataStorage() {
    static std::shared_ptr<DataStorage::IDataStorage> storage = std::make_shared<DataStorage>();
    return storage;
}

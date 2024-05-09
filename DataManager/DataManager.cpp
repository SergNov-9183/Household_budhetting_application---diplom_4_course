#include "DataManager.h"
#include "DataManager.hpp"

#include <algorithm>
#include <DataStorage.hpp>
#include "IDataStorage.h"

DataManager::DataManager::DataManager()
    : m_dataStorage(DataStorage::dataStorage())
    , m_categories(std::make_shared<Categories>())
    , m_accounts(std::make_shared<Accounts>())
    , m_operations(std::make_shared<Operations>())
{}

void DataManager::DataManager::setListener(Listener *listener) {
    if (m_listener != listener) {
        m_listener = listener;
    }
}

OpenProjectResult DataManager::DataManager::openProject(const std::string& fileName) {
    auto result = m_dataStorage->openProject(fileName);
    if (result != OpenProjectResult::Error) {
        m_categories->clear();
        m_accounts->clear();
        m_operations->clear();
        if (!m_dataStorage->loadCategories(m_categories) ||
            !m_dataStorage->loadAccounts(m_accounts) ||
            !m_dataStorage->loadOperations(m_operations)) {
            result = OpenProjectResult::Error;
        }
    }
    return result;
}

std::shared_ptr<Categories> DataManager::DataManager::categories() {
    return m_categories;
}

std::shared_ptr<Accounts> DataManager::DataManager::accounts() {
    return m_accounts;
}

std::shared_ptr<Operations> DataManager::DataManager::operations() {
    return m_operations;
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

void DataManager::DataManager::appendAccount(Account account) {
    int id;
    if (m_dataStorage->appendAccount(account, id)) {
        account.id = id;
        m_accounts->push_back(account);
        if (m_listener) {
            m_listener->onAccountAppend();
        }
    }
}

void DataManager::DataManager::appendOperation(Operation operation) {
    int id;
    if (m_dataStorage->appendOperation(operation, id)) {
        operation.id = id;
        m_operations->push_back(operation);
        if (m_listener) {
            m_listener->onOperationAppend();
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

void DataManager::DataManager::renameAccount(const std::string& name, int id) {
    if (auto account = std::find_if(m_accounts->begin(), m_accounts->end(), [id](const Account& account) { return account.id == id; });
        account != m_accounts->end() && m_dataStorage->renameAccount(name, id)) {
        account->name = name;
        if (m_listener) {
            m_listener->onAccountRenamed(id);
        }
    }
}

void DataManager::DataManager::changeOperation(const Operation& operation, int id) {
    if (m_dataStorage->changeOperation(operation, id)) {
        m_operations->push_back(operation);
        if (m_listener) {
            m_listener->onOperationChanged(id);
        }
    }
}

void DataManager::DataManager::deleteOperation(int id) {
    if (m_dataStorage->deleteOperation(id)) {
        m_operations->pop_back();
        if (m_listener) {
            m_listener->onOperationDeleted(id);
        }
    }
}

std::shared_ptr<DataManager::IDataManager> DataManager::dataManager() {
    static std::shared_ptr<DataManager::IDataManager> manager = std::make_shared<DataManager>();
    return manager;
}

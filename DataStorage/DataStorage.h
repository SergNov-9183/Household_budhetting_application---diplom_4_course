#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <IDataStorage.h>
#include "Database.h"

namespace DataStorage {
    class SqlCategories;
    class SqlAccounts;
    class SqlOperations;
    class DataStorage final : public IDataStorage{
    public:
        DataStorage();

        OpenProjectResult openProject(const std::string& fileName) override;

        bool loadCategories(std::shared_ptr<Categories>& categories) override;
        bool appendCategory(const Category& category, int& id) override;
        bool renameCategory(const std::string& name, int id) override;
        bool moveCategory(int parentId, int id) override;

        bool loadAccounts(std::shared_ptr<Accounts>& accounts) override;
        bool appendAccount(const Account& account, int& id) override;
        bool renameAccount(const std::string& name, int id) override;

        bool loadOperations(std::shared_ptr<Operations>& operations) override;
        bool appendOperation(const Operation& operation, int& id) override;
        bool changeOperation(const Operation& operation) override;
        bool removeOperation(int id) override;

    private:
        Database m_db;
        std::shared_ptr<SqlCategories> m_categories = nullptr;
        std::shared_ptr<SqlAccounts> m_accounts = nullptr;
        std::shared_ptr<SqlOperations> m_operations = nullptr;
    };
}

#endif // DATASTORAGE_H

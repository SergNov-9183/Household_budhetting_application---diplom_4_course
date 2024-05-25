#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include <memory>
#include <string>
#include "Categories.h"
#include "Accounts.h"
#include "Operations.h"
#include "Enums.h"

namespace DataStorage {
    class IDataStorage {
    public:
        virtual OpenProjectResult openProject(const std::string& fileName) = 0;

        virtual bool loadCategories(std::shared_ptr<Categories>& categories) = 0;
        virtual bool appendCategory(const Category& category, int& id) = 0;
        virtual bool renameCategory(const std::string& name, int id) = 0;
        virtual bool moveCategory(int parentId, int id) = 0;

        virtual bool loadAccounts(std::shared_ptr<Accounts>& accounts) = 0;
        virtual bool appendAccount(const Account& account, int& id) = 0;
        virtual bool renameAccount(const std::string& name, int id) = 0;

        virtual bool loadOperations(std::shared_ptr<Operations>& operations) = 0;
        virtual bool appendOperation(const Operation& operation, int& id) = 0;
        virtual bool changeOperation(const Operation& operation) = 0;
        virtual bool removeOperation(int id) = 0;
    };
}

#endif // IDATASTORAGE_H

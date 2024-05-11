#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <IDataManager.h>
#include "Categories.h"
#include "Accounts.h"
#include "Operations.h"

namespace DataStorage {
    class IDataStorage;
}

namespace DataManager {
    class DataManager final : public IDataManager {
    public:
        DataManager();

        void setListener(Listener* listener) override;
        OpenProjectResult openProject(const std::string& fileName) override;

        std::shared_ptr<Categories> categories() override;
        std::shared_ptr<Accounts> accounts() override;
        std::shared_ptr<Operations> operations() override;

        void appendCategory(Category category) override;
        void renameCategory(const std::string& name, int id) override;

        void appendAccount(Account account) override;
        void renameAccount(const std::string& name, int id) override;

        void appendOperation(Operation operation) override;
        void changeOperation(const Operation& operation) override;
        void removeOperation(int id) override;


    private:
        std::shared_ptr<DataStorage::IDataStorage> m_dataStorage = nullptr;
        std::shared_ptr<Categories> m_categories = nullptr;
        std::shared_ptr<Accounts> m_accounts = nullptr;
        std::shared_ptr<Operations> m_operations = nullptr;
        Listener* m_listener = nullptr;
    };
}

#endif // DATAMANAGER_H

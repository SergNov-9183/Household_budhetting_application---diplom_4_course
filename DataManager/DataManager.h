#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <IDataManager.h>
#include "Categories.h"

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
        void appendCategory(Category category) override;
        void renameCategory(const std::string& name, int id) override;

    private:
        std::shared_ptr<DataStorage::IDataStorage> m_dataStorage = nullptr;
        std::shared_ptr<Categories> m_categories = nullptr;
        Listener* m_listener = nullptr;
    };
}

#endif // DATAMANAGER_H

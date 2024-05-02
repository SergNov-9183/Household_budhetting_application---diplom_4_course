#ifndef IDATAMANAGER_H
#define IDATAMANAGER_H

#include <memory>
#include <string>
#include <Categories.h>
#include "Enums.h"

namespace DataManager {
    class IDataManager {
    public:
        struct Listener {
            virtual void onCategoryAppend() = 0;
            virtual void onCategoryRenamed(int id) = 0;
        };

        virtual void setListener(Listener* listener) = 0;
        virtual OpenProjectResult openProject(const std::string& fileName) = 0;
        virtual std::shared_ptr<Categories> categories() = 0;
        virtual void appendCategory(Category category) = 0;
        virtual void renameCategory(const std::string& name, int id) = 0;
    };
}

#endif // IDATAMANAGER_H

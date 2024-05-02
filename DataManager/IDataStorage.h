#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include <memory>
#include <string>
#include "Categories.h"
#include "Enums.h"

namespace DataStorage {
    class IDataStorage {
    public:
        virtual OpenProjectResult openProject(const std::string& fileName) = 0;
        virtual bool loadCategories(std::shared_ptr<Categories>& categories) = 0;
        virtual bool appendCategory(const Category& category, int& id) = 0;
        virtual bool renameCategory(const std::string& name, int id) = 0;
    };
}

#endif // IDATASTORAGE_H

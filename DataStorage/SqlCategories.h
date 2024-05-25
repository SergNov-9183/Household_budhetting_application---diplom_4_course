#ifndef SQLCATEGORIES_H
#define SQLCATEGORIES_H

#include <memory>
#include "Categories.h"
#include "SqlBase.h"

namespace DataStorage {
    class SqlCategories final : public SqlBase {
    public:
        SqlCategories(const Database& db);

        bool insert(const std::string& name, int parentId = 0, bool income = false);
        bool select(std::shared_ptr<Categories> categories);
        bool updateName(const std::string& name, int id);
        bool move(int parentId, int id);

        bool create() override;

    private:
        bool initTable();
    };
}

#endif // SQLCATEGORIES_H

#include "Categories.h"

Category::Category(int id, const std::string& name, int level, bool income, int parentId) {
    this->id       = id;
    this->name     = name;
    this->level    = level;
    this->income   = income;
    this->parentId = parentId;
}

Category::Category(const Category& category) {
    id       = category.id;
    name     = category.name;
    level    = category.level;
    income   = category.income;
    parentId = category.parentId;
}

Category::~Category() {}

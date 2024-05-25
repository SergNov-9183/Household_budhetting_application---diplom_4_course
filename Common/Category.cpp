#include "Categories.h"

Category::Category(int id, const std::string& name, bool income, int parentId) {
    this->id       = id;
    this->name     = name;
    this->income   = income;
    this->parentId = parentId;
}

Category::Category(const Category& category) {
    id       = category.id;
    name     = category.name;
    income   = category.income;
    parentId = category.parentId;
}

Category::~Category() {}

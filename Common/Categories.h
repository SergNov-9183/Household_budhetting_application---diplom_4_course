#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <string>
#include <vector>
#include "CommonExport.h"

struct COMMON_EXPORT Category {
    Category(int id, const std::string& name, int level, bool income, int parentId = 0);
    Category(const Category& category);
    ~Category();
    int id;
    std::string name;
    int level;
    bool income;
    int parentId;
};
using Categories = std::vector<Category>;

#endif // CATEGORIES_H

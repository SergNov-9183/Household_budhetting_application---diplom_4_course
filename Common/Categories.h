#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <string>
#include <vector>
#include "CommonExport.h"

struct COMMON_EXPORT Category {
    int id;
    std::string name;
    int level;
    bool income;
    int parentId;
};
using Categories = std::vector<Category>;

#endif // CATEGORIES_H

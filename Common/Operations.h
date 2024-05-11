#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include <vector>
#include "CommonExport.h"

struct COMMON_EXPORT Operation {
    Operation(int id, const std::string& description, int categoryId, int accountId, const std::string& date, float price);
    Operation(const Operation& operation);
    ~Operation();
    int id;
    std::string description;
    int categoryId;
    int accountId;
    std::string date;
    float price;
};
using Operations = std::vector<Operation>;

#endif // OPERATIONS_H

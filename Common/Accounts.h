#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <string>
#include <vector>
#include "CommonExport.h"

struct COMMON_EXPORT Account {
    Account(int id, const std::string& name, int type, int parentId = 0);
    Account(const Account& account);
    ~Account();
    int id;
    std::string name;
    int type;
    int parentId;
};
using Accounts = std::vector<Account>;

#endif // ACCOUNTS_H

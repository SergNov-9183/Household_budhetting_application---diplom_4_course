#ifndef SQLACCOUNTS_H
#define SQLACCOUNTS_H

#include <memory>
#include "Accounts.h"
#include "SqlBase.h"

namespace DataStorage {
class SqlAccounts final : public SqlBase {
public:
    SqlAccounts(const Database& db);

    bool insert(const std::string& name, int type, int parentId = 0);
    bool select(std::shared_ptr<Accounts> accounts);
    bool updateName(const std::string& name, int id);

    bool create() override;

private:
    bool initTable();
};
}

#endif // SQLACCOUNTS_H

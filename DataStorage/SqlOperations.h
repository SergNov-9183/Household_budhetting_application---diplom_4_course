#ifndef SQLOPERATIONS_H
#define SQLOPERATIONS_H

#include <memory>
#include "Operations.h"
#include "SqlBase.h"

namespace DataStorage {
class SqlOperations final : public SqlBase {
public:
    SqlOperations(const Database& db);

    bool insert(const std::string& name, int categoryId, int accountId, std::string date, float price);
    bool select(std::shared_ptr<Operations> operations);
    bool update(const std::string& description, int categoryId, float price, int id);
    bool insert(int id);
    bool remove(int id);

    bool create() override;
};
}

#endif // SQLOPERATIONS_H

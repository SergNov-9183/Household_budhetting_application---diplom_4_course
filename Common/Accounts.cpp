#include "Accounts.h"

Account::Account(int id, const std::string& name, int type, int parentId) {
    this->id       = id;
    this->name     = name;
    this->type     = type;
    this->parentId = parentId;
}

Account::Account(const Account& account) {
    id       = account.id;
    name     = account.name;
    type     = account.type;
    parentId = account.parentId;
}

Account::~Account() {}

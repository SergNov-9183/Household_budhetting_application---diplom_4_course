#include "Operations.h"

Operation::Operation(int id, const std::string& description, int categoryId, int accountId, const std::string& date, float price) {
    this->id          = id;
    this->description = description;
    this->categoryId  = categoryId;
    this->accountId   = accountId;
    this->date        = date;
    this->price       = price;
}

Operation::Operation(const Operation &operation) {
    id          = operation.id;
    description = operation.description;
    categoryId  = operation.categoryId;
    accountId   = operation.accountId;
    date        = operation.date;
    price       = operation.price;
}

Operation::~Operation() {}


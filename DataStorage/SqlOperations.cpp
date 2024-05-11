#include "SqlOperations.h"

namespace {
static const std::string table_name          = "operations";
static const std::string fieldDescription    = "description";
static const std::string fieldCategoryId     = "categoryId";
static const std::string fieldAccountId      = "accountId";
static const std::string fieldBuyDate        = "buyDate";
static const std::string fieldPrice          = "price";
}

DataStorage::SqlOperations::SqlOperations(const Database &db) : SqlBase(db, table_name) {}

bool DataStorage::SqlOperations::insert(const std::string& description, int categoryId, int accountId, const std::string& date, float price) {
    std::map<std::string, std::string> data;
    data[fieldDescription] = prepareStringValue(description);
    data[fieldCategoryId]  = std::to_string(categoryId);
    data[fieldAccountId]   = std::to_string(accountId);
    data[fieldBuyDate]     = prepareStringValue(date);
    data[fieldPrice]       = std::to_string(price);
    return SqlBase::insert(data);
}

bool DataStorage::SqlOperations::remove(int id) {
    return SqlBase::remove(id);
}

bool DataStorage::SqlOperations::select(std::shared_ptr<Operations> operations) {
    if (operations) {
        qslSelectFields fields = { idField(), fieldDescription, fieldCategoryId, fieldAccountId, fieldBuyDate, fieldPrice };
        qslSelectResult values;
        if (SqlBase::select(fields, values)) {
            for (auto i = 0; i < values.size(); ++i) {
                if (values[i].size() != fields.size()) {
                    return false;
                }
                auto id          = std::stoi(values[i][0]);
                auto description = values[i][1];
                auto categoryId  = std::stoi(values[i][2]);
                auto accountId   = std::stoi(values[i][3]);
                auto date        = values[i][4];
                auto price       = std::stof(values[i][5]);
                operations->push_back({id, description, categoryId, accountId, date, price});
            }
            return true;
        }
    }
    return false;
}

bool DataStorage::SqlOperations::update(const std::string& description, int categoryId, float price, int id) {
    std::map<std::string, std::string> data;
    data[fieldDescription] = prepareStringValue(description);
    data[fieldCategoryId]  = std::to_string(categoryId);
    data[fieldPrice]       = std::to_string(price);
    return SqlBase::update(data, id);
}

bool DataStorage::SqlOperations::create() {
    static const std::vector<std::pair<std::string, std::string>> fieldsInfo =
        { {fieldDescription, "text not null"}, {fieldCategoryId, "integer not null"}, {fieldAccountId, "integer not null default (0)"}, {fieldBuyDate, "text not null"}, {fieldPrice, "real not null"} };
    return createTable(fieldsInfo);
}

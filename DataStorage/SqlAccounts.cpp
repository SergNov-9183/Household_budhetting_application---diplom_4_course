#include "SqlAccounts.h"

namespace {
    static const std::string table_name    = "accounts";
    static const std::string fieldName     = "name";
    static const std::string fieldType     = "type";
    static const std::string fieldParentId = "parentId";
}

DataStorage::SqlAccounts::SqlAccounts(const Database &db) : SqlBase(db, table_name) {}

bool DataStorage::SqlAccounts::insert(const std::string& name, int type, int parentId) {
    std::map<std::string, std::string> data;
    data[fieldName]     = prepareStringValue(name);
    data[fieldType]     = std::to_string(type);
    data[fieldParentId] = std::to_string(parentId);
    return SqlBase::insert(data);
}

bool DataStorage::SqlAccounts::select(std::shared_ptr<Accounts> accounts) {
    if (accounts) {
        qslSelectFields fields = { idField(), fieldName, fieldType, fieldParentId };
        qslSelectResult values;
        if (SqlBase::select(fields, values)) {
            for (auto i = 0; i < values.size(); ++i) {
                if (values[i].size() != fields.size()) {
                    return false;
                }
                auto id       = std::stoi(values[i][0]);
                auto name     = values[i][1];
                auto type     = std::stoi(values[i][2]);
                auto parentId = std::stoi(values[i][3]);
                accounts->push_back({id, name, type, parentId});
            }
            return true;
        }
    }
    return false;
}

bool DataStorage::SqlAccounts::updateName(const std::string &name, int id) {
    std::map<std::string, std::string> data;
    data[fieldName] = prepareStringValue(name);
    return SqlBase::update(data, id);
}

bool DataStorage::SqlAccounts::create() {
    static const std::vector<std::pair<std::string, std::string>> fieldsInfo =
        { {fieldName, "text not null"}, {fieldType, "integer not null"}, {fieldParentId, "integer not null default (0)"} };
    return createTable(fieldsInfo) && initTable();
}

bool DataStorage::SqlAccounts::initTable() {
    return insert("Наличные деньги", 0) && insert("Банковские счета", 1) && insert("Дебетовые карты", 2);
}

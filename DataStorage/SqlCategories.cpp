#include "SqlCategories.h"

namespace {
    static const std::string table_name    = "categories";
    static const std::string fieldName     = "name";
    static const std::string fieldLevel    = "level";
    static const std::string fieldIncome   = "income";
    static const std::string fieldParentId = "parentId";
}

DataStorage::SqlCategories::SqlCategories(const Database &db) : SqlBase(db, table_name) {}

bool DataStorage::SqlCategories::insert(const std::string& name, int level, int parentId, bool income) {
    std::map<std::string, std::string> data;
    data[fieldName]     = prepareStringValue(name);
    data[fieldLevel]    = std::to_string(level);
    data[fieldIncome]   = std::to_string(income ? 1 : 0);
    data[fieldParentId] = std::to_string(parentId);
    return SqlBase::insert(data);
}

bool DataStorage::SqlCategories::select(std::shared_ptr<Categories> categories) {
    if (categories) {
        qslSelectFields fields = { idField(), fieldName, fieldLevel, fieldIncome, fieldParentId };
        qslSelectResult values;
        if (SqlBase::select(fields, values)) {
            for (auto i = 0; i < values.size(); ++i) {
                if (values[i].size() != fields.size()) {
                    return false;
                }
                auto id       = std::stoi(values[i][0]);
                auto name     = values[i][1];
                auto level    = std::stoi(values[i][2]);
                auto income   = values[i][3] == "1" ? true : false;
                auto parentId = std::stoi(values[i][4]);
                categories->push_back({id, name, level, income, parentId});
            }
            return true;
        }
    }
    return false;
}

bool DataStorage::SqlCategories::updateName(const std::string &name, int id) {
    std::map<std::string, std::string> data;
    data[fieldName]     = prepareStringValue(name);
    return SqlBase::update(data, id);
}

bool DataStorage::SqlCategories::create() {
    static const std::vector<std::pair<std::string, std::string>> fieldsInfo =
        { {fieldName, "text not null"}, {fieldLevel, "integer not null"}, {fieldIncome, "integer not null default (0)"}, {fieldParentId, "integer not null default (0)"} };
    return createTable(fieldsInfo) && initTable();
}

bool DataStorage::SqlCategories::initTable() {
    if (insert("Доходы", 0, 0, true) && insert("Зарплата", 1, lastId(), true) && insert("Расходы", 0)) {
        auto id = lastId();
        return insert("Аптека", 1, id) && insert("Бытовая химия", 1, id) && insert("Продукты", 1, id) && insert("Отдых", 1, id) && insert("Электроника", 1, id);
    }
    return false;
}

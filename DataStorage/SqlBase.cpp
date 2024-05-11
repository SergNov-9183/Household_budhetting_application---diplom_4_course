#include "SqlBase.h"

#include <sstream>
#include "SqlQuery.h"

namespace {
    static const std::string id_field              = "id";
    static const std::string ERROR_MESSAGE_READ_ID = "Get value data from database is failed, id does not exist: ";
}

DataStorage::SqlBase::SqlBase(const Database& db, const std::string& tableName) : m_db(db), m_tableName(tableName) {}

int DataStorage::SqlBase::lastId() const {
    return m_lastId;
}

const std::string &DataStorage::SqlBase::idField() const {
    return id_field;
}

bool DataStorage::SqlBase::lastIdIsValid() const {
    return m_lastId > 0;
}

bool DataStorage::SqlBase::readLastId() {
    const std::string sql = "select max(" + id_field + ") from " + m_tableName;

    SqlQuery query(m_db);
    switch (query.executeScalar(sql, m_lastId)) {
    case SqlQuery::Result::Ok:
        return true;
    case SqlQuery::Result::Empty:
        errorMessage = std::string(ERROR_MESSAGE_READ_ID).append(m_tableName);
        break;
    default:
        errorMessage = query.lastError();
        break;
    }
    m_lastId = -1;
    return false;
}

bool DataStorage::SqlBase::createTable(const std::vector<std::pair<std::string, std::string>>& fieldsInfo) {
    auto sql = buildCreateTableSql(m_tableName, fieldsInfo);
    SqlQuery query(m_db);
    if (query.executeCommand(sql)) {
        return true;
    }
    else {
        errorMessage = query.lastError();
    }
    return false;
}

bool DataStorage::SqlBase::insert(const std::map<std::string, std::string>& data) {
    std::stringstream fields;
    std::stringstream values;
    for (auto it = data.begin(); it != data.end();) {
        fields << it->first;
        values << it->second;
        if (++it != data.end()) {
            fields << ", ";
            values << ", ";
        }
    }
    std::stringstream sql;
    sql << "insert into " << m_tableName << "(" << fields.str() << ") values (" << values.str() << ");";
    SqlQuery query(m_db);
    if (!query.executeCommand(sql.str())) {
        errorMessage = query.lastError();
        return false;
    }
    return readLastId();
}

bool DataStorage::SqlBase::select(const qslSelectFields& fields, qslSelectResult& values) {
    std::stringstream sql;
    sql << "select ";
    for (auto i = 0; i < fields.size(); ++i) {
        sql << fields[i] << (i == fields.size() - 1 ? "" : ", ");
    }
    sql << " from " << m_tableName;

    SqlQuery query(m_db);

    if (query.execute(sql.str())) {
        while (true) {
            auto nextResult = query.next();
            if (nextResult == SqlQuery::Result::Ok) {
                sqlRecord record;
                for (auto i = 0; i < fields.size(); ++i) {
                    record.push_back(query.fieldValueAsString(i));
                }
                values.push_back(record);
            }
            else if (nextResult == SqlQuery::Result::Empty) {
                break;
            }
            else {
                errorMessage = query.lastError();
                return false;
            }
        }
    }
    else {
        errorMessage = query.lastError();
        return false;
    }
    return true;
}

bool DataStorage::SqlBase::update(const std::map<std::string, std::string>& data, int id) {
    std::stringstream sql;
    sql << "update " << m_tableName << " set ";
    for (auto it = data.begin(); it != data.end();) {
        sql << it->first << " = " << it->second;
        if (++it != data.end()) {
            sql << ", ";
        }
    }
    sql << " where " << id_field << " = " << id;
    SqlQuery query(m_db);
    if (!query.executeCommand(sql.str())) {
        errorMessage = query.lastError();
        return false;
    }
    return true;
}

bool DataStorage::SqlBase::remove(int id) {
    std::stringstream sql;
    sql << "delete from " << m_tableName << " where " << id_field << " = " << id;
    SqlQuery query(m_db);
    if (!query.executeCommand(sql.str())) {
        errorMessage = query.lastError();
        return false;
    }
    return true;
}

std::string DataStorage::SqlBase::prepareStringValue(const std::string& value) {
    auto result = value;
    for (auto i = result.size(); i > 0; --i) {
        if (result[i - 1] == '\'') {
            result.insert(i - 1, "'");
        }
    }
    result.insert(0, "'");
    result.insert(result.end(), '\'');
    return result;
}

std::string DataStorage::SqlBase::buildCreateTableSql(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& fieldsInfo) {
    static std::pair<std::string, std::string> idFieldInfo = {id_field, "integer primary key autoincrement"};
    std::stringstream result;
    result << "create table if not exists " << tableName << "(";
    auto addField = [&result](const std::string& fieldName, const std::string& fieldInfo, bool isLast = false) {
        result << fieldName << " " << fieldInfo << (isLast ? ");" : ", ");
    };
    addField(idFieldInfo.first, idFieldInfo.second);
    auto lastIndex = fieldsInfo.size() - 1;
    for (auto i = 0; i <= lastIndex; ++i) {
        addField(fieldsInfo[i].first, fieldsInfo[i].second, i == lastIndex);
    }
    return result.str();
}

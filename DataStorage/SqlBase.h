#ifndef SQLBASE_H
#define SQLBASE_H

#include <map>
#include <vector>

#include "Database.h"

namespace DataStorage {
    using sqlRecord       = std::vector<std::string>;
    using qslSelectResult = std::vector<sqlRecord>;
    using qslSelectFields = std::vector<std::string>;
    class SqlBase {
    public:
        SqlBase(const Database& db, const std::string& tableName);

        int lastId() const;
        const std::string& idField() const;
        bool lastIdIsValid() const;

        virtual bool create() = 0;

    protected:
        bool readLastId();
        bool createTable(const std::vector<std::pair<std::string, std::string>>& fieldsInfo);
        bool insert(const std::map<std::string, std::string>& data);
        bool select(const qslSelectFields& fields, qslSelectResult& values);
        bool update(const std::map<std::string, std::string>& data, int id);

        static std::string prepareStringValue(const std::string& value);

    private:
        static std::string buildCreateTableSql(const std::string& tableName, const std::vector<std::pair<std::string, std::string>>& fieldsInfo);

        const Database& m_db;
        std::string m_tableName;
        int m_lastId = -1;
        std::string errorMessage = std::string();
    };
}


#endif // SQLBASE_H

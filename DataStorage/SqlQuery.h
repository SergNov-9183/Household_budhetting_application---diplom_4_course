#ifndef SQL_QUERY_HPP
#define SQL_QUERY_HPP

#include "Database.h"
#include <string>

namespace DataStorage {
    class SqlQuery {
    public:
        enum class Result {
            Error,
            Ok,
            Empty
        };

        SqlQuery(const Database& database);
        SqlQuery()                = delete;
        SqlQuery(const SqlQuery&) = delete;
        SqlQuery(SqlQuery&&)      = delete;
        ~SqlQuery();

        void beginTransaction();
        void endTransaction(bool commit = true);

        bool execute(const std::string& query);
        bool executeCommand(const std::string& query);
        std::string lastError() const;
        Result next();

        std::string fieldValueAsString(int index = 0);
        int fieldValueAsInt(int index = 0);

        template <typename ValueType>
        Result executeScalar(const std::string& query, ValueType& value) {
            if (execute(query)) {
                auto result = next();
                if (result == Result::Ok) {
                    fieldValue(value);
                }
                else {
                    value = ValueType();
                }
                finalizeStmt();
                return result;
            }
            else {
                finalizeStmt();
                return Result::Error;
            }
        }

    private:
        sqlite3* m_db;
        sqlite3_stmt* m_stmt;
        std::string m_lastError;

        void finalizeStmt();
        void fieldValue(std::string& value, int index = 0);
        void fieldValue(int& value, int index = 0);
    };
}

#endif /* SQL_QUERY_HPP */

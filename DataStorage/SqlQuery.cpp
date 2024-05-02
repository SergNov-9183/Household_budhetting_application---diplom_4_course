#include "SqlQuery.h"

namespace DataStorage {

    const std::string ERROR_MESSAGE_SELECT        = "Selecting data from database is failed, error code: ";
    const std::string ERROR_MESSAGE_SQL_ERROR     = "SQL error: ";
    const std::string ERROR_MESSAGE_UNKNOWN_ERROR = "Some error encountered, error code: ";

    SqlQuery::SqlQuery(const Database& database) :
        m_db(database.instance()), m_stmt(NULL), m_lastError(std::string()) {
    }

    SqlQuery::~SqlQuery() {
        finalizeStmt();
    }

    void SqlQuery::beginTransaction() {
        sqlite3_exec(m_db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    }

    void SqlQuery::endTransaction(bool commit) {
        sqlite3_exec(m_db, commit ? "END TRANSACTION;" : "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
    }

    bool SqlQuery::execute(const std::string& query) {
        finalizeStmt();

        int result = sqlite3_prepare_v2(m_db, query.c_str(), -1, &m_stmt, 0);
        if (result) {
            m_lastError = std::string(ERROR_MESSAGE_SELECT).append(std::to_string(result));
            return false;
        }
        else {
            m_lastError = std::string();
            return true;
        }
    }

    bool SqlQuery::executeCommand(const std::string& query) {
        char* zErrMsg = 0;
        if (sqlite3_exec(m_db, query.c_str(), 0, 0, &zErrMsg) != SQLITE_OK) {
            m_lastError = std::string(zErrMsg);
            m_lastError.insert(0, ERROR_MESSAGE_SQL_ERROR);
            sqlite3_free(zErrMsg);
            return false;
        }
        else {
            m_lastError = std::string();
            return true;
        }
    }

    std::string SqlQuery::lastError() const {
        return m_lastError;
    }

    SqlQuery::Result SqlQuery::next() {
        auto result = sqlite3_step(m_stmt);
        switch (result) {
            case SQLITE_ROW:
                return Result::Ok;
            case SQLITE_DONE:
                finalizeStmt();
                return Result::Empty;
            default:
                m_lastError = std::string(ERROR_MESSAGE_UNKNOWN_ERROR).append(std::to_string(result));
                finalizeStmt();
                return Result::Error;
        }
    }

    std::string SqlQuery::fieldValueAsString(int index) {
        auto value = reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, index));
        return value ? std::string(value) : std::string();
    }

    int SqlQuery::fieldValueAsInt(int index) {
        return sqlite3_column_int(m_stmt, index);
    }

    void SqlQuery::finalizeStmt() {
        if (m_stmt != NULL) {
            sqlite3_finalize(m_stmt);
            m_stmt = NULL;
        }
    }

    void SqlQuery::fieldValue(std::string& value, int index) {
        value = fieldValueAsString(index);
    }

    void SqlQuery::fieldValue(int& value, int index) {
        value = fieldValueAsInt(index);
    }
}

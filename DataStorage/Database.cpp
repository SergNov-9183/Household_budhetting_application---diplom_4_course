#include "Database.h"

#include "Utils.h"

namespace DataStorage {

    const char* SQLITE_PRAGMA_SYNC_OFF    = "PRAGMA synchronous = OFF";
    const char* SQLITE_PRAGMA_JOURNAL_MEM = "PRAGMA journal_mode = MEMORY";

    Database::~Database() {
        close();
    }

    sqlite3* Database::instance() const {
        return m_db;
    }

    const std::string& Database::path() const { return m_fileName; }

    OpenProjectResult Database::open(const std::string& fileName) {
        m_fileName = fileName;
        auto result = Utils::fileExists(m_fileName.c_str()) ? OpenProjectResult::Opened : OpenProjectResult::Created;
        if (sqlite3_open(m_fileName.c_str(), &m_db)) {
            m_isOpen = false;
            return OpenProjectResult::Error;
        }

        char* zErrMsg;
        if (sqlite3_exec(m_db, SQLITE_PRAGMA_SYNC_OFF, nullptr, nullptr, &zErrMsg) != SQLITE_OK) {
            sqlite3_free(zErrMsg);
            zErrMsg = nullptr;
        }

        if (sqlite3_exec(m_db, SQLITE_PRAGMA_JOURNAL_MEM, nullptr, nullptr, &zErrMsg) != SQLITE_OK) {
            sqlite3_free(zErrMsg);
            zErrMsg = nullptr;
        }

        m_isOpen   = true;
        m_fileName = std::string(sqlite3_db_filename(m_db, "main"));
        return result;
    }

    void Database::close() {
        if (m_isOpen) {
            m_isOpen = false;
            sqlite3_close(m_db);
        }
    }

    bool Database::isOpen() const {
        return m_isOpen;
    }
}

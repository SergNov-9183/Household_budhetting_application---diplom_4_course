#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

#include "Enums.h"

namespace DataStorage {

    class Database {
    public:
        Database()                = default;
        Database(const Database&) = delete;
        Database(Database&&)      = delete;
        ~Database();

        sqlite3* instance() const;
        const std::string& path() const;

        OpenProjectResult open(const std::string& fileName);
        void close();
        bool isOpen() const;

    private:
        std::string m_fileName;
        sqlite3* m_db;
        bool m_isOpen;
    };
}

#endif /* DATABASE_H */

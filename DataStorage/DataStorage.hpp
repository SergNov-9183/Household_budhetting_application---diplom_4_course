#ifndef DATASTORAGE_HPP
#define DATASTORAGE_HPP

#include <memory>
#include "DataStorageExport.h"

namespace DataStorage {
    class IDataStorage;
    extern "C++" DATASTORAGE_EXPORT std::shared_ptr<IDataStorage> dataStorage();
}

#endif // DATASTORAGE_HPP

#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <memory>
#include "DataManagerExport.h"

namespace DataManager {
    class IDataManager;
    extern "C++" DATAMANAGER_EXPORT std::shared_ptr<IDataManager> dataManager();
}

#endif // DATAMANAGER_HPP

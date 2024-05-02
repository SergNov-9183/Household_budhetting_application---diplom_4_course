#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "CommonExport.h"

struct COMMON_EXPORT Utils {
    static bool fileExists(const std::string& fileName);
};

#endif // UTILS_H

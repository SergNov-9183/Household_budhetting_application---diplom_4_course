#include "Utils.h"

#include <sys/stat.h>

bool Utils::fileExists(const std::string& fileName) {
    struct stat buffer;
    return stat(fileName.c_str(), &buffer) == 0;
}

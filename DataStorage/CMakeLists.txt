cmake_minimum_required(VERSION 3.14)

project(DataStorage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${ROOT_SOURCE_DIR}/DataManager/
    ${ROOT_SOURCE_DIR}/ThirdParty/include/
)

add_library(DataStorage SHARED
    DataStorageExport.h
    DataStorage.cpp
    DataStorage.h
    DataStorage.hpp
    Database.cpp Database.h
    SqlQuery.cpp SqlQuery.h
    SqlBase.h SqlBase.cpp
    SqlCategories.h SqlCategories.cpp
)

target_link_libraries(DataStorage PRIVATE Common sqlite3)

target_compile_definitions(DataStorage PRIVATE DATASTORAGE_LIBRARY)

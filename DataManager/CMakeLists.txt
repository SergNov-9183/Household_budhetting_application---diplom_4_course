cmake_minimum_required(VERSION 3.14)

project(DataManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${ROOT_SOURCE_DIR}/HomeFinance/
    ${ROOT_SOURCE_DIR}/DataStorage/
)

add_library(DataManager SHARED
    IDataStorage.h
    DataManagerExport.h
    DataManager.cpp
    DataManager.h
    DataManager.hpp
)

target_link_libraries(DataManager PRIVATE DataStorage)

target_compile_definitions(DataManager PRIVATE DATAMANAGER_LIBRARY)

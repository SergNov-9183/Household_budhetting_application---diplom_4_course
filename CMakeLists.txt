cmake_minimum_required(VERSION 3.16)

project(HomeFinance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SOURCE_SQLITE_DLL ${ROOT_SOURCE_DIR}/ThirdParty/bin/sqlite3.dll)
set(TARGET_SQLITE_DLL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sqlite3.dll)

include_directories(
    ${ROOT_SOURCE_DIR}/Common/
    ${ROOT_SOURCE_DIR}/DataManager/
)

link_directories(
    ${ROOT_SOURCE_DIR}/ThirdParty/bin
)

add_subdirectory(HomeFinance)
add_subdirectory(DataManager)
add_subdirectory(DataStorage)
add_subdirectory(Common)
add_subdirectory(TestsForHomeFinance)

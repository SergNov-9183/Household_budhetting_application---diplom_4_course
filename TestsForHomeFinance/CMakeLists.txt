cmake_minimum_required(VERSION 3.5)

project(TestsForHomeFinance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${ROOT_SOURCE_DIR}/DataManager/
    ${ROOT_SOURCE_DIR}/DataStorage/
    ${ROOT_SOURCE_DIR}/HomeFinance/
    ${ROOT_SOURCE_DIR}/ThirdParty/include/
)

add_executable(TestsForHomeFinance main.cpp)

target_link_libraries(TestsForHomeFinance PRIVATE DataManager DataStorage Common)

include(GNUInstallDirs)
install(TARGETS TestsForHomeFinance
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

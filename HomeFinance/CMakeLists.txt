cmake_minimum_required(VERSION 3.16)

project(HomeFinance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    ${ROOT_SOURCE_DIR}/DataManager/
)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appHomeFinance
    main.qrc
    main.cpp
)

qt_add_qml_module(appHomeFinance
    URI HomeFinance
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES Style.qml
    QML_FILES CategoriesList.qml
    QML_FILES IconButton.qml
    SOURCES IDataManager.h
    SOURCES EditorController.h EditorController.cpp
    SOURCES CategoriesModel.h CategoriesModel.cpp
    SOURCES CategoriesProxyModel.h CategoriesProxyModel.cpp
    QML_FILES FrameItem.qml
    QML_FILES ListViewDelegate.qml
    QML_FILES HFLabel.qml
    QML_FILES EditLabel.qml
    QML_FILES HFDialogWindow.qml
    QML_FILES TextButton.qml
    QML_FILES AddCategoryDialog.qml
    QML_FILES HFEditText.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appHomeFinance PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appHomeFinance
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appHomeFinance PRIVATE Qt6::Quick DataManager DataStorage Common)

include(GNUInstallDirs)
install(TARGETS appHomeFinance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET appHomeFinance POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_SQLITE_DLL} ${TARGET_SQLITE_DLL})

cmake_minimum_required(VERSION 3.16)

project(HomeFinance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
    ${ROOT_SOURCE_DIR}/DataManager/
)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Quick Qml Gui Widgets)

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
    QML_FILES HFEditText.qml
    QML_FILES BorderLine.qml
    QML_FILES CategoriesDialog.qml
    QML_FILES SettingsDialog.qml
    QML_FILES OperationsSection.qml
    QML_FILES AccountsDialog.qml
    QML_FILES AnalyticsSection.qml
    QML_FILES DisableFrame.qml
    SOURCES HFSettings.h HFSettings.cpp
    SOURCES AccountsModel.h AccountsModel.cpp
    SOURCES AccountsProxyModel.h AccountsProxyModel.cpp
    QML_FILES AccountsList.qml
    QML_FILES ExpandingButton.qml
    QML_FILES HFTabButton.qml
    QML_FILES ExpandingMenu.qml
    QML_FILES TabsRow.qml
    SOURCES AccountsTabsProxyModel.h AccountsTabsProxyModel.cpp
    QML_FILES OperationsList.qml
    SOURCES OperationsModel.h OperationsModel.cpp
    SOURCES OperationsProxyModel.h OperationsProxyModel.cpp
    QML_FILES LabelButton.qml
    QML_FILES CategoriesPopup.qml
    QML_FILES EditLabelWithIcon.qml
    SOURCES AnalyticsProxyModel.h AnalyticsProxyModel.cpp
    QML_FILES PeriodPicker.qml
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

target_link_libraries(appHomeFinance PRIVATE Qt6::Core Qt6::Quick Qt6::Qml Qt6::Gui Qt6::Widgets DataManager DataStorage Common)

include(GNUInstallDirs)
install(TARGETS appHomeFinance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET appHomeFinance POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_SQLITE_DLL} ${TARGET_SQLITE_DLL})

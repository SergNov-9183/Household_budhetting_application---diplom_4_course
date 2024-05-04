import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import HomeFinance

Window {
    id: root

    property CategoriesModel categoriesModel: CategoriesModel {
        editorController: globalController
    }

    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
    visible: true
    title: qsTr("Домашние финансы")
    color: Style.colors.backgroundWindow

    Component.onCompleted: globalController.load()

    Connections {
        target: globalController
        function onHasProjectChanged(value) {
            if (value) {
                sectionList.currentIndex = 1
            }
        }
        function onNeedCreateOrOpenProject() {
            fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog

        property bool change: false

        acceptLabel: qsTr("Открыть/Создать")
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        fileMode: FileDialog.SaveFile
        nameFilters: ["Фыйлы приложения (*.hfproject)", "Фыйлы SQLite (*.sqlite3)", "Все файлы (*)"]
        rejectLabel: change ? qsTr("отмена") : qsTr("Закрыть программу")
        title: qsTr("Необходимо выбрать проект или создать новый")

        function changeProject() {
            change = true
            open()
        }

        onAccepted: globalController.createOrOpen(selectedFile)
        onRejected: {
            if (!change) {
                Qt.quit()
            }
        }
    }

    LinearGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1C2125" }
            GradientStop { position: 1.0; color: "#21201A" }
        }
    }

    UserFrame {
        id: userInfo
        anchors {
            left: parent.left
            top: parent.top
            margins: Style.margins.base
        }
        onClicked: sectionList.currentIndex = 4
    }

    BorderLine {
        id: leftArea
        anchors {
            left: userInfo.right
            leftMargin: Style.margins.base
            top: parent.top
            bottom: statusBar.top
        }
        isHorizontal: false
    }

    BorderLine {
        id: userArea
        anchors {
            left: parent.left
            top: userInfo.bottom
            topMargin: Style.margins.base
            right: leftArea.left
        }
    }

    SectionList {
        id: sectionList
        anchors {
            left: parent.left
            top: userArea.bottom
            right: leftArea.left
            bottom: statusBar.top
            margins: Style.margins.base
        }
        onClicked: (index) => mainArea.currentIndex = index
    }

    FrameItem {
        id: mainFrame
        anchors {
            left: leftArea.right
            top: parent.top
            right: parent.right
            bottom: statusBar.top
            margins: Style.margins.base
        }

        StackLayout {
            id: mainArea
            anchors {
                fill: parent
                margins: Style.margins.base
            }

            AccountsSection   {}
            OperationsSection {}
            CategoriesSection {}
            AnalyticsSection  {}
            SettingsSection   {}
        }
    }

    Item {
        id: statusBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 30

        BorderLine {
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
        }
    }

    DisableFrame {
        visible: !globalController.hasProject || fileDialog.visible || addCategoryWindow.visible || addAccountWindow.visible
    }
    AddCategoryDialog { id: addCategoryWindow }

    AddAccountDialog { id: addAccountWindow }
}
